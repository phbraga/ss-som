/* 
 * File:   SOMType.h
 * Author: hans
 *
 * Created on 30 de Março de 2012, 13:15
 */

#ifndef CLUSTERINGSOM_H
#define	CLUSTERINGSOM_H

#include <vector>
#include <map>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "ArffData.h"
#include "MatVector.h"

#include "Mesh.h"
#include "SOM.h"
#include "DSNode.h"
#include "SSSOMNode.h"
#include "ClusteringMetrics.h"


template <class SOMType>
class ClusteringSOM {

public:
    SOMType *som;
    MatMatrix<float> *trainingData;
    bool allocated;
    bool sorted;
    std::vector<int> groups;
    std::map<int, int> groupLabels; 
    
    std::vector<int> supervisedGroup;
    std::vector<int> reinforcementGroup;
    
    bool isSubspaceClustering;
    bool filterNoise;

public:
    ClusteringSOM(){
        isSubspaceClustering = true;
        trainingData = NULL;
        allocated = false;        
        filterNoise = true;
    };

    ClusteringSOM(SOMType *som) {
        this->som = som;
        isSubspaceClustering = true;
        trainingData = NULL;
        allocated = false;        
        filterNoise = true;
    };

    virtual ~ClusteringSOM(){
        if (allocated)
            delete trainingData;
    };

    void setFilterNoise(bool filter) {
        filterNoise = filter;
    }
    
    void setIsSubspaceClustering(bool isSubspaceClustering) {
        this->isSubspaceClustering = isSubspaceClustering;
    }

    virtual int getMeshSize() = 0;

    int getInputSize() {
        return trainingData->cols();
    }

    int getNumSamples() {
        return trainingData->rows();
    }

    virtual int getNodeId(int node_i) {
        return node_i;
    }

    virtual void train(MatMatrix<float> &trainingData, int epochs) = 0;

    virtual void getRelevances(int node_i, MatVector<float> &relevances) = 0;

    virtual void getWeights(int node_i, MatVector<float> &weights) = 0;

    virtual void getWinners(const MatVector<float> &sample, std::vector<int> &winners) = 0;
    
    virtual int getWinner(const MatVector<float> &sample) = 0;

    virtual bool isNoise(float activation) {
        return false;
    }
    
    void cleanUpTrainingData() {
        
        if (allocated) {
            delete trainingData;
            trainingData = NULL;
            allocated = false;
        }
        
        groups.clear();
        groupLabels.clear();
    }

    bool readFile(const std::string &filename, bool normalize) {

        if (trainingData == NULL && !allocated) {
            trainingData = new MatMatrix<float>();
            allocated = true;
        }

        if (ArffData::readArff(filename, *trainingData, groupLabels, groups)) {
            if (normalize) {
                ArffData::rescaleCols01(*trainingData);
            }
            return true;
        }
        return false;
    }

    void setData(MatMatrix<float> &data) {
        if (allocated) {
            delete trainingData;
            allocated = false;
        }

        trainingData = &data;
    }

    void trainSOM(int epochs) {

        train(*trainingData, epochs);
    }
    
    virtual bool writeClusterResults(const std::string &filename) {  
    }

    std::string printConditionalEntropy(std::vector<int> &groups) {
        std::stringstream out;
        MatVector<int> trueClusters(groups.size());
        MatVector<int> obtained(groups.size());
        int noise = 0;

        for (int k = 0; k < trainingData->rows(); k++) {
            MatVector<float> sample;
            trainingData->getRow(k, sample);

            //            if (isNoise(sample)) {
            //                noise++;
            //                continue;
            //            }

            int w = getWinner(sample);

            trueClusters[k] = groups[k];
            obtained[k] = w;
            //dbgOut(0) << trueClusters[k] << "\t" << obtained[k] << endl;

        }

        out << "Conditional entropy:\t" << conditionalEntropy(trueClusters, obtained) << endl;
        out << "Total noise:\t" << noise << "(" << noise / (float) trainingData->rows() << ")" << endl;
        return out.str();
    }

    float conditionalEntropy(MatVector<int> &X, MatVector<int> &Y) {
        std::map<int, int> m;

        for (int i = 0; i < Y.size(); i++) {
            int cluster = Y[i];

            if (m.find(cluster) != m.end()) {
                int count = m[cluster];
                m[cluster] = count + 1;
            } else {
                m[cluster] = 1;
            }
        }

        for (int i = 0; i < X.size(); i++) {
            int cluster = X[i];

            if (m.find(cluster) != m.end()) {
                int count = m[cluster];
                m[cluster] = count + 1;
            } else {
                m[cluster] = 1;
            }
        }

        int ny = m.size();

        MatVector<double> py(ny);
        MatMatrix<double> pxDy(ny, ny);
        py.fill(0);
        pxDy.fill(0);

        //Count y and (x,y) ocurrences
        for (int i = 0; i < Y.size(); i++) {
            pxDy[X[i]][Y[i]]++;
            py[Y[i]]++;
        }

        //Calculate p(x|y)
        for (int x = 0; x < ny; x++) {
            for (int y = 0; y < ny; y++) {
                if (py[y] > 0)
                    pxDy[x][y] /= py[y];
                else
                    pxDy[x][y] = 0;
            }
        }

        //Calculate p(y)
        for (int y = 0; y < ny; y++) {
            py[y] /= Y.size();
        }

        //dbgOut(0) << py.toString() << endl;
        //Compute conditional entropy: H(X|Y) = sum_y{py*sum_x[p(x|y)*log(1/p(x|y))]}
        double hxDy = 0;
        for (int y = 0; y < ny; y++) {

            float sum_pxDylog1_pxDy = 0;
            for (int x = 0; x < ny; x++) {
                if (pxDy[x][y] > 0)
                    sum_pxDylog1_pxDy += pxDy[x][y] * log2(1 / pxDy[x][y]);
            }
            hxDy += py[y] * sum_pxDylog1_pxDy;
        }

        return hxDy;
    }
};

#ifdef SOM2D_H

class ClusteringSOM2D : public ClusteringSOM<SOM2D<DSNeuron, SOMParameters> > {
public:

    ClusteringSOM2D(SOM2D<DSNeuron, SOMParameters> *som) : ClusteringSOM<SOM2D<DSNeuron, SOMParameters> >(som) {
    };

    int getMeshSize() {
        return som->getSomCols() * som->getSomRows();
    }

    void train(MatMatrix<float> &trainingData, int N) {
        som->parameters.NFeatures = trainingData.cols();
        som->parameters.tmax = N;
        som->initializeP(som->parameters);
        som->train(trainingData);
    }

    void getRelevances(int node_i, MatVector<float> &relevances) {
        relevances.size(som->parameters.NFeatures);
        relevances.fill(1);
    }

    void getWeights(int node_i, MatVector<float> &weights) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        DSNeuron neuron(row, col);
        som->getNeuron(neuron);

        weights = neuron.weights;
    }

    int getWinner(const MatVector<float> &sample) {

        DSNeuron bmu;
        som->findBMU(sample, bmu);
        return bmu.c + bmu.r * som->getSomCols();
    }

    void getWinners(const MatVector<float> &sample, std::vector<int> &winner) {

        winner.clear();

        DSNeuron bmu;
        som->findBMU(sample, bmu);
        winner.push_back(bmu.c + bmu.r * som->getSomCols());
    }
};
#endif

#ifdef DSLVQ_H

class ClusteringDSLVQ : public ClusteringSOM<DSSOM<LVQNeuron, DSSOMParameters> > {
public:

    ClusteringDSLVQ(DSSOM<LVQNeuron, DSSOMParameters> *som) : ClusteringSOM<DSSOM<LVQNeuron, DSSOMParameters> >(som) {
    };

    int getMeshSize() {
        return som->getSomCols() * som->getSomRows();
    }

    void train(MatMatrix<float> &trainingData, int N) {
        som->parameters.NFeatures = trainingData.cols();
        som->parameters.tmax = N;
        som->initializeP(som->parameters);
        som->train(trainingData);
    }

    void test(MatMatrix<float> &testData, MatVector<int> classes) {

        classes.size(testData.rows());

        for (int r = 0; r < testData.rows(); r++) {
            MatVector<float> sample;
            testData.getRow(r, sample);

            LVQNeuron neuron;
            som->findBMU(sample, neuron);
            classes[r] = neuron.getClass();
        }
    }

    void getRelevances(int node_i, MatVector<float> &relevances) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        LVQNeuron neuron(row, col);
        som->getNeuron(neuron);

        relevances = neuron.dsWeights;
    }

    void getWeights(int node_i, MatVector<float> &weights) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        LVQNeuron neuron(row, col);
        som->getNeuron(neuron);

        weights = neuron.weights;
    }

    int getWinner(const MatVector<float> &sample) {

        LVQNeuron bmu;
        som->findBMU(sample, bmu);
        return bmu.c + bmu.r * som->getSomCols();
    }

    void getWinners(const MatVector<float> &sample, std::vector<int> &winner) {

        winner.clear();

        LVQNeuron bmu;
        som->findBMU(sample, bmu);
        winner.push_back(bmu.c + bmu.r * som->getSomCols());
    }
};
#endif

#ifdef _DSSOMC_H

class ClusteringDSSOMC : public ClusteringSOM<DSSOMC<DSCNeuron, DSSOMCParameters> > {
public:

    ClusteringDSSOMC(DSSOMC<DSCNeuron, DSSOMCParameters> *som) : ClusteringSOM<DSSOMC<DSCNeuron, DSSOMCParameters> >(som) {
    };

    int getMeshSize() {
        return som->getSomCols() * som->getSomRows();
    }

    void train(MatMatrix<float> &trainingData, int N) {
        som->parameters.NFeatures = trainingData.cols();
        som->parameters.tmax = N;
        som->initializeP(som->parameters);
        som->train(trainingData);
    }

    void getRelevances(int node_i, MatVector<float> &relevances) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        DSCNeuron neuron(row, col);
        som->getNeuron(neuron);

        relevances = neuron.dsWeights;
    }

    void getWeights(int node_i, MatVector<float> &weights) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        DSCNeuron neuron(row, col);
        som->getNeuron(neuron);

        weights = neuron.weights;
    }

    int getWinner(const MatVector<float> &sample) {

        DSCNeuron bmu;
        som->findBMU(sample, bmu);
        return bmu.c + bmu.r * som->getSomCols();
    }

    void getWinners(const MatVector<float> &sample, std::vector<int> &winner) {

        winner.clear();

        DSCNeuron bmu;
        som->findBMU(sample, bmu);
        winner.push_back(bmu.c + bmu.r * som->getSomCols());
    }
};
#endif

#ifdef _SOMAW_H

class ClusteringSOMAW : public ClusteringSOM<SOMAW<DSNeuron, SOMAWParameters> > {
public:

    ClusteringSOMAW(SOMAW<DSNeuron, SOMAWParameters> *som) : ClusteringSOM<SOMAW<DSNeuron, SOMAWParameters> >(som) {
    };

    int getMeshSize() {
        return som->getSomCols() * som->getSomRows();
    }

    void train(MatMatrix<float> &trainingData, int N) {
        som->parameters.NFeatures = trainingData.cols();
        som->parameters.tmax = N;
        som->initializeP(som->parameters);
        som->train(trainingData);
    }

    void getRelevances(int node_i, MatVector<float> &relevances) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        DSNeuron neuron(row, col);
        som->getNeuron(neuron);

        relevances = neuron.dsWeights;
    }

    void getWeights(int node_i, MatVector<float> &weights) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        DSNeuron neuron(row, col);
        som->getNeuron(neuron);

        weights = neuron.weights;
    }

    int getWinner(const MatVector<float> &sample) {

        DSNeuron bmu;
        som->findBMU(sample, bmu);
        return bmu.c + bmu.r * som->getSomCols();
    }

    void getWinners(const MatVector<float> &sample, std::vector<int> &winner) {

        winner.clear();

        DSNeuron bmu;
        som->findBMU(sample, bmu);
        winner.push_back(bmu.c + bmu.r * som->getSomCols());
    }
};
#endif

#ifdef _DSSOM_H

class ClusteringDSSOM : public ClusteringSOM<DSSOM<DSNeuron, DSSOMParameters> > {
public:

    ClusteringDSSOM(DSSOM<DSNeuron, DSSOMParameters> *som) : ClusteringSOM<DSSOM<DSNeuron, DSSOMParameters> >(som) {
    };

    int getMeshSize() {
        return som->getSomCols() * som->getSomRows();
    }

    void train(MatMatrix<float> &trainingData, int N) {
        som->parameters.NFeatures = trainingData.cols();
        som->parameters.tmax = N;
        som->initializeP(som->parameters);
        som->train(trainingData);
    }

    void getRelevances(int node_i, MatVector<float> &relevances) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        DSNeuron neuron(row, col);
        som->getNeuron(neuron);

        relevances = neuron.dsWeights;
    }

    void getWeights(int node_i, MatVector<float> &weights) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        DSNeuron neuron(row, col);
        som->getNeuron(neuron);

        weights = neuron.weights;
    }

    int getWinner(const MatVector<float> &sample) {

        DSNeuron bmu;
        som->findBMU(sample, bmu);
        return bmu.c + bmu.r * som->getSomCols();
    }

    void getWinners(const MatVector<float> &sample, std::vector<int> &winner) {

        winner.clear();

        DSNeuron bmu;
        som->findBMU(sample, bmu);
        winner.push_back(bmu.c + bmu.r * som->getSomCols());
    }
};
/*
class ClusteringDSSOM: public ClusteringSOM<DSSOM> {

public:
    ClusteringDSSOM(DSSOM *som) : ClusteringSOM<DSSOM>(som){};
    
    int getMeshSize() {
        return som->getSomCols() * som->getSomRows();
    }

    void train(MatMatrix<float> &trainingData, int N) {
        som->parameters.NFeatures = trainingData.cols();
        som->parameters.tmax = N;
        som->initializeP(som->parameters);
        som->train(trainingData);
    }
    
    void getRelevances(int node_i, MatVector<float> &relevances) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        DSNeuron neuron(row, col);
        som->getNeuron(neuron);

        relevances = neuron.dsWeights;
    }

    void getWeights(int node_i, MatVector<float> &weights) {
        int col = node_i / som->getSomRows();
        int row = ((node_i / (float) som->getSomRows()) - col) * som->getSomRows();

        DSNeuron neuron(row, col);
        som->getNeuron(neuron);

        weights = neuron.dsWeights;
    }
    
    int getWinner(const MatVector<float> &sample) {
        
        DSNeuron bmu;
        som->findFirstBMU(sample, bmu);
        return bmu.c + bmu.r*som->getSomCols();
    }
        
    void getWinners(const MatVector<float> &sample, std::vector<int> &winner) {
    
        DSNeuron bmu;
        som->resetRelevances();
        int k=1;
        while (som->getRelevance().max()>som->parameters.epsilonRho && k<=som->parameters.numWinners)
        {
            if (k==1) {
                float activation = som->findFirstBMU(sample, bmu);
                if (activation < som->parameters.outliersThreshold) {
                    winner.push_back(-1);
                    break;
                }
            }
            else {
                som->findNextBMU(sample, bmu);
            }
            winner.push_back(bmu.c + bmu.r*som->getSomCols());
            som->updateRelevances(bmu);
            k = k + 1;
        }
    }
};
 */
#endif

#ifdef SOM_H_

class ClusteringMeshSSSOM : public ClusteringSOM<SOM<SSSOMNode>> {
public:

    ClusteringMeshSSSOM(SOM<SSSOMNode> *som) : ClusteringSOM<SOM<SSSOMNode> >(som) {
    };

    int getMeshSize() {
        return som->meshNodeSet.size();
    }

    std::vector<int> getNodeClasses() {
        std::vector<int> nodeClasses;
        SOM<SSSOMNode>::TPNodeSet::iterator it = som->meshNodeSet.begin();
        int i = 0;
        for (; it != som->meshNodeSet.end(); it++, i++) {
            nodeClasses.push_back((*it)->cls);
        }
        
        return nodeClasses;
    }
    
    std::vector<int> getNodeIds() {
        std::vector<int> nodeIds;
        SOM<SSSOMNode>::TPNodeSet::iterator it = som->meshNodeSet.begin();
        int i = 0;
        for (; it != som->meshNodeSet.end(); it++, i++) {
            nodeIds.push_back((*it)->getId());
        }
        
        return nodeIds;
    }
    
    void train(MatMatrix<float> &trainingData, int epochs) {
        som->data = trainingData;

        if (!sorted) {
            som->trainning(epochs, groups, groupLabels);
        } else {
            som->orderedTrainning(epochs, groups, groupLabels);
        }
    }

    void getRelevances(int node_i, MatVector<float> &relevances) {
        SOM<SSSOMNode>::TPNodeSet::iterator it = som->meshNodeSet.begin();
        int i = 0;
        for (; it != som->meshNodeSet.end(); it++, i++) {
            if (i == node_i) {
                relevances = (*it)->ds;
                return;
            }
        }
        return;
    }

    void getWeights(int node_i, MatVector<float> &weights) {
        SOM<SSSOMNode>::TPNodeSet::iterator it = som->meshNodeSet.begin();
        int i = 0;
        for (; it != som->meshNodeSet.end(); it++, i++) {
            if (i == node_i) {
                weights = (*it)->w;
                return;
            }
        }
        return;
    }

    int getWinner(const MatVector<float> &sample) {
        SSSOMNode *winner = som->getWinner(sample);
        return getNodeIndex(*winner);
    }
    
    std::vector<int> getWinnerResult(const MatVector<float> &sample) {
        SSSOMNode *winner = som->getWinnerResult(sample);
        std::vector<int> result;
        result.push_back(getNodeIndex(*winner));
        result.push_back(winner->cls);
        result.push_back(winner->getId());
        result.push_back(winner->act);
        
        return result;
    }

    void getWinners(const MatVector<float> &sample, std::vector<int> &winners) {
        SSSOMNode *winner = som->getFirstWinner(sample);
        winners.push_back(getNodeIndex(*winner));

        winner = som->getNextWinner(winner);
        while (winner != NULL) {
            winners.push_back(getNodeIndex(*winner));
            winner = som->getNextWinner(winner);
        }
    }

    virtual bool isNoise(float activation) {
        return som->isNoise(activation);
    }

    int getNodeIndex(SSSOMNode &node) {
        SOM<SSSOMNode>::TPNodeSet::iterator it = som->meshNodeSet.begin();
        int i = 0;
        for (; it != som->meshNodeSet.end(); it++, i++) {
            if ((*it) == &node) {
                return i;
            }
        }
        return -1;
    }

    int getNodeId(int node_i) {
        SOM<SSSOMNode>::TPNodeSet::iterator it = som->meshNodeSet.begin();
        int i = 0;
        for (; it != som->meshNodeSet.end(); it++, i++) {
            if (i == node_i) {
                return (*it)->getId();
            }
        }
        return -1;
    }
    
    bool readFile(const std::string &filename, bool normalize) {

        if (trainingData == NULL && !allocated) {
            trainingData = new MatMatrix<float>();
            allocated = true;
        }

        if (ArffData::readArffClass(filename, *trainingData, groupLabels, groups)) {
            if (normalize) {
                ArffData::rescaleCols01(*trainingData);
            }
            return true;
        }
        return false;
    }
    
    bool writeClusterResults(const std::string &filename) {
        std::ofstream file;
        file.open(filename.c_str());

        if (!file.is_open()) {
            dbgOut(0) << "Error openning output file" << endl;
            return false;
        }

        int meshSize = getMeshSize();
        int inputSize = getInputSize();

        file << meshSize << "\t" << inputSize << endl;

        for (int i = 0; i < meshSize; i++) {
            MatVector<float> relevances;
            getRelevances(i, relevances);

            file << i << "\t";
            for (int j = 0; j < inputSize; j++) {
                file << relevances[j];
                if (j != inputSize - 1)
                    file << "\t";
            }
            file << endl;
        }

        for (int i = 0; i < trainingData->rows(); i++) {
            MatVector<float> sample;
            trainingData->getRow(i, sample);

            std::vector<int> winners;
            std::vector<int> result;
            result = getWinnerResult(sample);
            winners.push_back(result[0]);
            
            if (filterNoise && isNoise(result[3]))
                continue;

            for (int j = 0; j < winners.size(); j++) {
                file << i << "\t";
                file << winners[j];
                
                if (result.size() > 0) {
                    file<< "\t" << result[1];
                }
                
                file << endl;
            }
        }

        file.close();
        return true;
    }

    std::string outConfusionMatrix(std::vector<int> &groups, std::map<int, int> &groupLabels) {

        std::stringstream out;
        out << std::setprecision(2) << std::fixed;

        int meshSize = getMeshSize();
        MatMatrix<int> confusionMatrix(getMeshSize(), groupLabels.size());
        confusionMatrix.fill(0);
        int noise = 0;

        for (int k = 0; k < trainingData->rows(); k++) {
            MatVector<float> sample;
            trainingData->getRow(k, sample);

            std::vector<int> winners;
            std::vector<int> idss;
            std::vector<int> result;
            
            result = getWinnerResult(sample);
            winners.push_back(result[0]);
            idss.push_back(result[2]);
                
            
            if (filterNoise && isNoise(result[3])) {
                noise++;
                continue;
            }

            for (int w = 0; w < winners.size(); w++) {
                confusionMatrix[winners[w]][groups[k]]++;
            }
        }

        /** print confusion matrix **/
        MatVector<int> rowSums(confusionMatrix.rows());
        MatVector<int> colSums(confusionMatrix.cols());
        
        std::vector<int> cluClasses = getNodeClasses();
        std::vector<int> cluIds = getNodeIds();
        
        rowSums.fill(0);
        colSums.fill(0);
        dbgOut(0) << "cluster\\class\t|";
        for (int c = 0; c < confusionMatrix.cols(); c++)
            dbgOut(0) << "\tcla" << groupLabels[c];
        dbgOut(0) << "\t| Sum" << endl;
        for (int r = 0; r < confusionMatrix.rows(); r++) {
            dbgOut(0) << "Node " << cluIds[r] << " (" << cluClasses[r] <<")\t|";
            for (int c = 0; c < confusionMatrix.cols(); c++) {
                dbgOut(0) << "\t" << confusionMatrix[r][c];
                rowSums[r] += confusionMatrix[r][c];
                colSums[c] += confusionMatrix[r][c];
            }
            dbgOut(0) << "\t| " << rowSums[r] << endl;
        }
        dbgOut(0) << "Sums\t\t|";
        for (int c = 0; c < confusionMatrix.cols(); c++)
            dbgOut(0) << "\t" << colSums[c];
        dbgOut(0) << "\t| " << colSums.sum() << endl << endl;
        /***/

        dbgOut(0) << "Random index: " << ClusteringMetrics::RANDI(confusionMatrix) << endl;
        dbgOut(0) << "Adjusted random index: " << ClusteringMetrics::ARI(confusionMatrix) << endl << endl;
        out << "Total noise:\t" << noise << "(" << noise / (float) trainingData->rows() << ")" << endl;

        return out.str();
    }

    std::string outClassInfo(std::vector<int> &groups, std::map<int, int> &groupLabels) {

        std::stringstream out;
        out << std::setprecision(2) << std::fixed;

        int meshSize = getMeshSize();
        int hits = 0;
        int noise = 0;

        MatVector<int> nodeHits(meshSize);
        MatVector<int> nodeClusterSize(meshSize);
        nodeHits.fill(0);
        nodeClusterSize.fill(0);

        for (int k = 0; k < groups.size(); k++) {
            MatVector<float> sample;
            trainingData->getRow(k, sample);

            std::vector<int> result = getWinnerResult(sample);
            int winner = result[0];
            
            
            if (filterNoise && isNoise(result[3])) {
                noise++;
                continue;
            }
//            MatVector<float> weights;
//            getWeights(winner, weights);
            
//            dbgOut(1) << groups[k] << endl;
            
            int cls = result[1];
            if (fabs(groups[k] - cls) < 0.5) {
                hits++;
                nodeHits[winner]++;
            }

            nodeClusterSize[winner]++;
        }
        
        for (int i = 0; i < meshSize; i++) {

            MatVector<float> weights;
            getWeights(i, weights);

            dbgOut(0) << getNodeId(i) << ": ";
//            out << "\t" << weights[weights.size() - 1];
            dbgOut(0) << "\t" << nodeHits[i] << "/" << nodeClusterSize[i];
            
            float sizeDiv = 0;
            
            if (nodeClusterSize[i] > 0) {
                sizeDiv = nodeHits[i] / (float) nodeClusterSize[i];
            }
            dbgOut(0) << "\t" << sizeDiv;
            dbgOut(0) << endl;
        }

        dbgOut(0) << "Classification acuracy:\t" << hits / (float) trainingData->rows() << endl;
        dbgOut(0) << "Total noise:\t" << noise / (float) trainingData->rows() << endl;

        return out.str();
    }
};

#endif

#endif /* CLUSTERINGSOM_H */
