/*
 *  Created on: 2017
 *      Author: Pedro Magalhaes
 */

#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <limits>
#include <unistd.h>
#include "Defines.h"
#include "DebugOut.h"
#include "ClusteringMetrics.h"
#include "ArffData.h"
#include "ClusteringSOM.h"
#include "SSSOM.h"
#include "DisplaySSSOM.h"
#include <sys/stat.h>

void runExperiments (std::vector<float> params, string filePath, string outputPath,
        bool isFilterNoise, bool sorted, bool normalize, bool keepMapSaved);
void runTrainTestExperiments (std::vector<float> params, string filePath, string testPath, string outputPath, 
        bool isFilterNoise, bool sorted, bool displayMap, bool normalize, bool keepMapSaved);
void evaluate (string filePath, string somPath, bool filterNoise, bool sorted, bool normalize);

std::vector<float> loadParametersFile(string path);
std::vector<string> loadStringFile(string path);
string getFileName(string filePath);
int findLast(const string str, string delim);

int main(int argc, char** argv) {

    dbgThreshold(0);

    dbgOut(0) << "Running SS-SOM" << endl;

    string inputPath = "";
    string testPath = "";
    string resultPath = "";
    string mapPath = "";
    string parametersFile = "";

    bool isFilterNoise = true;
    bool isSorted = false;
    
    bool runTrainTest = false;
    bool runEvaluation = false;
    
    bool displayMap = false;
    
    bool normalize = false;
    
    bool keepMapSaved = false;
    
    int c;
    while ((c = getopt(argc, argv, "i:t:r:p:m:sfScednk")) != -1) {

        switch (c) {
            case 'i':
                inputPath.assign(optarg);
                break;
            case 't':
                testPath.assign(optarg);
                break;
            case 'r':
                resultPath.assign(optarg);
                break;
            case 'p':
                parametersFile.assign(optarg);
                break;
            case 'm':
                mapPath.assign(optarg);
                break;
            case 'f':
                isFilterNoise = false;
                break;
            case 'S':
                isSorted = true;
                break;
            case 'c':
                runTrainTest = true;
                break;
            case 'e':
                runEvaluation = true;
                break;
            case 'd':
                displayMap = true;
                break;
            case 'n':
                normalize = true;
                break;
            case 'k':
                keepMapSaved = true;
                break;
        }
    }

    if (runEvaluation) {
        evaluate(inputPath, mapPath, isFilterNoise, isSorted, normalize);
        return 0;
    }
    
    if (mkdir(resultPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
        dbgOut(0) << "Results Directory Created" << endl;
    } 
    
    std::vector<string> inputFiles = loadStringFile(inputPath);
    std::vector<string> testFiles = loadStringFile(testPath);
    std::vector<float> params = loadParametersFile(parametersFile);

    for (int i = 0 ; i < inputFiles.size() - 1 ; ++i) {
        if(!runTrainTest) {
            runExperiments(params, inputFiles[i], resultPath, isFilterNoise, isSorted, normalize, keepMapSaved);
        } else {
            runTrainTestExperiments(params, inputFiles[i], testFiles[i], resultPath, isFilterNoise, isSorted, displayMap, normalize, keepMapSaved);
        }
    }
}

void evaluate (string filePath, string somPath, bool filterNoise, bool sorted, bool normalize) {
    SSSOM som(1);
    SOM<SSSOMNode> *sssom = (SOM<SSSOMNode>*) &som;
    som.noCls = 999;
    som.readSOM(somPath);
    
    ClusteringMeshSSSOM clusteringSOM(sssom);
    clusteringSOM.readFile(filePath, normalize);
    clusteringSOM.sorted = sorted;
    
    
    clusteringSOM.setIsSubspaceClustering(false);
    clusteringSOM.setFilterNoise(filterNoise);
    
    clusteringSOM.outConfusionMatrix(clusteringSOM.groups, clusteringSOM.groupLabels);
    clusteringSOM.outClassInfo(clusteringSOM.groups, clusteringSOM.groupLabels);
}

void runExperiments (std::vector<float> params, string filePath, string outputPath,
        bool isFilterNoise, bool sorted, bool normalize, bool keepMapSaved) {

    SSSOM som(1);
    SOM<SSSOMNode> *sssom = (SOM<SSSOMNode>*) &som;

    ClusteringMeshSSSOM clusteringSOM(sssom);
    clusteringSOM.readFile(filePath, normalize);
    clusteringSOM.sorted = sorted;

    clusteringSOM.setIsSubspaceClustering(false);
    clusteringSOM.setFilterNoise(isFilterNoise);    
    
    int numberOfParameters = 11;
    
    for (int i = 0 ; i < params.size() - 1 ; i += numberOfParameters) {
        som.a_t = params[i];
        som.lp = params[i + 1];
        som.dsbeta = params[i + 2];
        som.age_wins = params[i + 3];
        som.e_b = params[i + 4];
        som.e_n = params[i + 5] * som.e_b;
        som.epsilon_ds = params[i + 6];
        som.minwd = params[i + 7];
        som.epochs = params[i + 8];
        som.push_rate = params[i + 9] * som.e_b;
                
        string index = std::to_string((i/numberOfParameters));
        
        srand(params[i + 10]);
        
        som.noCls = 999;
        som.maxNodeNumber = clusteringSOM.getNumSamples();
        som.age_wins = round(som.age_wins*clusteringSOM.getNumSamples());
        som.reset(clusteringSOM.getInputSize());
        clusteringSOM.trainSOM(som.epochs);
        som.finishMapFixed(sorted, clusteringSOM.groups, clusteringSOM.groupLabels);
        
        if (keepMapSaved) {
            som.saveSOM(outputPath + "som_" + getFileName(filePath) + "_" + index);
        }
        
        clusteringSOM.writeClusterResults(outputPath + getFileName(filePath) + "_" + index + ".results");
    }
}

void runTrainTestExperiments (std::vector<float> params, string filePath, string testPath, string outputPath,
        bool isFilterNoise, bool sorted, bool displayMap, bool normalize, bool keepMapSaved) { 
    
    int numberOfParameters = 11;
    
    for (int i = 0 ; i < params.size() - 1 ; i += numberOfParameters) {
        SSSOM som(1);
        SOM<SSSOMNode> *sssom = (SOM<SSSOMNode>*) &som;

        ClusteringMeshSSSOM clusteringSOM(sssom);
        clusteringSOM.readFile(filePath, normalize);
        clusteringSOM.sorted = sorted;

        clusteringSOM.setIsSubspaceClustering(false);
        clusteringSOM.setFilterNoise(isFilterNoise);   
                        
        som.a_t = params[i];
        som.lp = params[i + 1];
        som.dsbeta = params[i + 2];
        som.age_wins = params[i + 3];
        som.e_b = params[i + 4];
        som.e_n = params[i + 5] * som.e_b;
        som.epsilon_ds = params[i + 6];
        som.minwd = params[i + 7]; 
        som.epochs = params[i + 8];
        som.push_rate = params[i + 9] * som.e_b;
        
        string index = std::to_string((i/numberOfParameters));
                  
        srand(params[i + 10]);

        som.noCls = 999;
        som.maxNodeNumber = clusteringSOM.getNumSamples();
        som.age_wins = round(som.age_wins*clusteringSOM.getNumSamples());
        som.reset(clusteringSOM.getInputSize());
        
        if (displayMap) {
            
            DisplaySSSOM dm(&som, clusteringSOM.trainingData);
            som.data = *clusteringSOM.trainingData;
            dm.setTrueClustersData(clusteringSOM.groups);
            dm.setGroupLabels(&clusteringSOM.groupLabels);
            dm.setTrueClustersColor(true);
            
            for (int epoch = 0; epoch < som.epochs; epoch++) {
                for (int row = 0 ; row < clusteringSOM.trainingData->rows() ; ++row) {
                    if (sorted) {
                        som.trainningStep(row, clusteringSOM.groups, clusteringSOM.groupLabels);
                    } else {
                        som.trainningStep(rand()%clusteringSOM.trainingData->rows(), 
                                clusteringSOM.groups, clusteringSOM.groupLabels);
                    }
                    
                    dm.display();
                }
            }
            
            som.finishMapFixed(sorted, clusteringSOM.groups, clusteringSOM.groupLabels);
            
            if (keepMapSaved) {
                som.saveSOM(outputPath + "som_" + getFileName(filePath) + "_" + index);
            }
            
            dm.displayLoop();
            
            clusteringSOM.cleanUpTrainingData();
            clusteringSOM.readFile(testPath, normalize);
            clusteringSOM.writeClusterResults(outputPath + getFileName(testPath) + "_" + index + ".results");
        
            dm.setTrainingData(clusteringSOM.trainingData);
            dm.setTrueClustersData(clusteringSOM.groups);
            dm.setGroupLabels(&clusteringSOM.groupLabels);

            dm.displayLoop();
            dm.close();
            
        } else {
            clusteringSOM.trainSOM(som.epochs);
            
            som.finishMapFixed(sorted, clusteringSOM.groups, clusteringSOM.groupLabels);
            
            if (keepMapSaved) {
                som.saveSOM(outputPath + "som_" + getFileName(filePath) + "_" + index);
            }
            
            clusteringSOM.cleanUpTrainingData();
            clusteringSOM.readFile(testPath, normalize);
            clusteringSOM.writeClusterResults(outputPath + getFileName(testPath) + "_" + index + ".results");
        }
    }
}

std::vector<float> loadParametersFile(string path) {
    std::ifstream file(path.c_str());
    std::string text;
    std::vector<float> params;
    while (!file.eof()) {
        getline(file, text);
        params.push_back(std::atof(text.c_str()));
    }
    return params;
}

std::vector<string> loadStringFile(string path) {
    std::ifstream file(path.c_str());
    std::string text;
    std::vector<string> params;
    while (!file.eof()) {
        getline(file, text);
        params.push_back(text.c_str());
    }
    return params;
}

string getFileName(string filePath) {

    int start = findLast(filePath, "/");
    int end = findLast(filePath, ".");

    return filePath.substr(start + 1, end - start - 1);
}

int findLast(string str, string delim) {
    std::vector<int> splits;

    int current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        splits.push_back(current);
        previous = current + 1;
        current = str.find(delim, previous);
    }

    return splits[splits.size() - 1];
}
