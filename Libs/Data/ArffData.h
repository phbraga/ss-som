/* 
 * File:   ArffData.h
 * Author: hans
 *
 * Created on 20 de Março de 2012, 14:31
 */

#ifndef ARFFDATA_H
#define	ARFFDATA_H

#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
#include "MatMatrix.h"
#include "DebugOut.h"

class ArffData {
public:

    int nColumns;
    int lineNumber;
    std::ifstream *file;
    int classIndex;
    
    ArffData(const std::string filename, int classIndex = -1) {
        file = new std::ifstream(filename.c_str());
        this->classIndex = classIndex;
                
        if (file->is_open()) {
                nColumns = -1;
                lineNumber = 0;
        } else {
            delete file;
            file = NULL;
        }
    }
    
    bool isOpen() {
        if (file==NULL) 
            return false;
        return file->is_open();
    }
    
    bool rewind() {
        file->clear();                 // clear fail and eof bits
        file->seekg(0, std::ios::beg); // back to the start!
    }
    
    bool getNextRow(MatVector<float> &row, std::map<int, int> &groupLabels, std::vector<int> &groups) {
        
        float d;
        int col;
        int group;
        row.clear();
        std::string line = "";
        
        //Compute nColumns
        if (nColumns<0) {
            
            getline(*file, line, '\n');
            std::stringstream parser(line);
            if (file->eof()) return false;
            col=0;
            
            while (true) {
                  parser >> d;
                  if (parser.fail()) break;
                  if (parser.eof()) break;
                  if (parser.bad()) break;                      
                  col++;
            }
            
            nColumns = col;
        }
        
        if (row.size()!=nColumns) {
            row.size(nColumns);
        }
        
        do {
            lineNumber++;
            
            if (line.size()<1)
                getline(*file, line, '\n');
           
            if (file->eof()) return false;
            
            if (line.size() < 1 || line[0] == '@')
                continue;

            if (!hasDigits(line))
                continue;
            
            if (line[line.size()-1] == '\r')
                line = line.substr(0, line.size()-1);

            std::stringstream parser(line);
            float temp;
            parser >> d;
            col=0;
            int index = classIndex;
            while (true) {

                if (col >= nColumns) {
                    dbgOut(0) << "Wrong number of columns in file at line: " << lineNumber << endl;
                    break;
                }
                
                parser >> temp;

                if (parser.fail()) {//try again, may be another separator
                    parser.clear();
                    char ch;
                    parser >> ch;
                    parser >> temp;
                }
                
                if (parser.eof()) {//Read group
                    row[col]=d;
                    col++;
                    
                    if (classIndex < 0 || classIndex == col)
                        group = temp;
                    else {
                        row[col]=temp;
                        col++;
                    }
                    
                    break;
                }
                
                if (!parser.bad()) {
                    
                    if (col == index) {
                        group = d;
                        index = -1;
                    }
                    else {
                        row[col]=d;
                        col++;
                    }
                    d = temp;                    
                }
            }
            
            dbgOut(2) << row.toString() << "\t" << group << endl;

            if (nColumns != col) {
                dbgOut(0) << "Wrong number of columns in file at line: " << lineNumber << endl;
                return false;
            }

            bool itemFound = false;
            int itemIndex;
            for (std::map<int, int>::iterator it = groupLabels.begin(); it != groupLabels.end(); it++) {
                if (it->second == group) {
                    itemFound = true;
                    itemIndex = it->first;
                    break;
                }
            }

            if (!itemFound) {
                itemIndex = groupLabels.size();
                groupLabels[itemIndex] = group;
            }

            groups.push_back(itemIndex);

            return true;
        } while (true);

        return false;
    }
    
    bool readArff(MatMatrix<float> &data, std::map<int, int> &groupLabels, std::vector<int> &groups) {

        float d;
        int group;
        int lineNumber = 1;
        int r=0;
        std::string line = "";
        int col;
        
        //Compute nColumns
        if (nColumns<0) {
            
            getline(*file, line, '\n');
            std::stringstream parser(line);
            if (file->eof()) return false;
            col=0;
            
            while (true) {
                  parser >> d;
                  if (parser.fail()) break;
                  if (parser.eof()) break;
                  if (parser.bad()) break;                      
                  col++;
            }
            
            nColumns = col;
        }
        
        data.resize(data.rows(), nColumns);

        do {
            if (file->eof()) break;
            
            if (line.size() < 1 || line[0] == '@')
                continue;

            if (!hasDigits(line))
                continue;
            
            if (line[line.size()-1] == '\r')
                line = line.substr(0, line.size()-1);

            std::stringstream parser(line);
            MatVector<float> row;            
            if (nColumns>0)
                row.size(nColumns);
            
            float temp;
            parser >> d;
            int index = classIndex;
            int col=0;
            while (true) {
                parser >> temp;

                if (parser.fail()) {//try again, may be a separator
                    parser.clear();
                    char ch;
                    parser >> ch;
                    parser >> temp;
                }

                if (parser.eof()) {
                    
                    if (!setRowValue(row, col, d)) {
                        dbgOut(0) << "Wrong number of columns in file at line: " << lineNumber << endl;
                        dbgOut(1) << nColumns << "!=" << row.size() << endl << row.toString() << endl;
                        return false;
                    }
                    col++;
                    
                    if (classIndex < 0 || classIndex == col)
                        group = temp; //Read group
                    else {
                        if (!setRowValue(row, col, d)){
                            dbgOut(0) << "Wrong number of columns in file at line: " << lineNumber << endl;
                            dbgOut(1) << nColumns << "!=" << row.size() << endl << row.toString() << endl;
                            return false;
                        }
                        col++;
                    }
                                        
                    group = temp;
                    break;
                }
                
                if (!parser.bad()) {
                    
                    if (col == index) {
                        group = d;
                        index = -1;
                    }
                    else {
                        if (!setRowValue(row, col, d)){
                            dbgOut(0) << "Wrong number of columns in file at line: " << lineNumber << endl;
                            dbgOut(1) << nColumns << "!=" << row.size() << endl << row.toString() << endl;
                            return false;
                        }
                        col++;
                   }
                   d = temp;
                }
            }
            
            dbgOut(3) << row.toString() << "\t" << group << endl;

            if (nColumns == -1)
                nColumns = row.size();
            else if (nColumns != row.size()) {
                dbgOut(0) << "Wrong number of columns in file at line: " << lineNumber << endl;
                dbgOut(1) << nColumns << "!=" << row.size() << endl << row.toString() << endl;
                return false;
            }

            bool itemFound = false;
            int itemIndex;
            for (std::map<int, int>::iterator it = groupLabels.begin(); it != groupLabels.end(); it++) {
                if (it->second == group) {
                    itemFound = true;
                    itemIndex = it->first;
                    break;
                }
            }

            if (!itemFound) {
                itemIndex = groupLabels.size();
                groupLabels[itemIndex] = group;
            }

            groups.push_back(itemIndex);

            if (r<data.rows())
                data.setRow(r,row);
            else {
                data.concatRows(row);
            }
            r++;
            
            lineNumber++;
            getline(*file, line, '\n');
            
        } while (true);

        return true;
    }    

    ~ArffData() {
        if (file!=NULL) {
                file->close();
                delete file;
        }
    }
    
    static bool readArffBD(const std::string filename, MatMatrix<float> &data, std::map<int, int> &groupLabels, std::vector<int> &groups) {
        std::ifstream file(filename.c_str());

        if (!file.is_open()) {
            dbgOut(0) << "Could not open file:" << filename << endl;
            return false;
        }
        
        int size = 0;
        int nColumns = -1;        
        std::string line; 
        float d;
        
        do {
            getline(file, line, '\n');
           
            if (file.eof()) break;
            
            if (line.size() < 1 || line[0] == '@')
                continue;

            if (!hasDigits(line))
                continue;
            
            if (line[line.size()-1] == '\r')
                line = line.substr(0, line.size()-1);
            
            size++;
            
            if (nColumns == -1) {
                std::stringstream parser(line);
                float temp;
                parser >> d;
                nColumns = 0;
                while (true) {
                    parser >> temp;

                    if (parser.fail()) {//try again, may be a separator
                        parser.clear();
                        char ch;
                        parser >> ch;
                        parser >> temp;
                    }

                    if (parser.eof()) {//Read group
                        nColumns++;
                        break;
                    }

                    if (!parser.bad()) {
                        nColumns++;
                    }
                }
            }
        } while (true);
        
        if (nColumns<0) {
            file.close();
            dbgOut(0) << "Error reading file: " << filename << ". Col count:" << nColumns << endl;
            return false;
        }
        
        if (data.size(size, nColumns).getVect()==NULL) {
            file.close();
            dbgOut(0) << "Error allocating matrix:" << filename << ". Row count:" << size << endl;
            return false;
        }

        file.clear();
        file.seekg(0);
        int group;       
        int lineNumber = 0;
        int col = 0;
        int row = 0;
        
        do {
            lineNumber++;
            getline(file, line, '\n');
           
            if (file.eof()) break;
            
            if (line.size() < 1 || line[0] == '@')
                continue;

            if (!hasDigits(line))
                continue;
            
            if (line[line.size()-1] == '\r')
                line = line.substr(0, line.size()-1);

            std::stringstream parser(line);
            float temp;
            parser >> d;
            col = 0;
            while (true) {
                parser >> temp;

                if (parser.fail()) {//try again, may be a separator
                    parser.clear();
                    char ch;
                    parser >> ch;
                    parser >> temp;
                }

                if (parser.eof()) {//Read group
                    data[row][col] = d;
                    col++;
                    group = temp;
                    break;
                }
                
                if (!parser.bad()) {
                    data[row][col] = d;
                    col++;
                    d = temp;
                }
            }

            if (nColumns != col) {
                file.close();
                dbgOut(0) << "Wrong number of columns in file " << filename << " at line: " << lineNumber << endl;
                return false;
            }

            bool itemFound = false;
            int itemIndex;
            for (std::map<int, int>::iterator it = groupLabels.begin(); it != groupLabels.end(); it++) {
                if (it->second == group) {
                    itemFound = true;
                    itemIndex = it->first;
                    break;
                }
            }

            if (!itemFound) {
                itemIndex = groupLabels.size();
                groupLabels[itemIndex] = group;
            }

            groups.push_back(itemIndex);

            row++;
        } while (true);
        file.close();

        return true;
    }
    
    static bool readArffClass(const std::string filename, MatMatrix<float> &data, std::map<int, int> &groupLabels, std::vector<int> &groups) {

        std::ifstream file(filename.c_str());

        if (!file.is_open()) {
            dbgOut(0) << "Could not open file:" << filename << endl;
            return false;
        }

        float d;
        int nColumns = -1;
        int group;
        std::string line;
        int lineNumber = 0;

        do {
            lineNumber++;
            getline(file, line, '\n');
           
            if (file.eof()) break;
            
            if (line.size() < 1 || line[0] == '@')
                continue;

            if (!hasDigits(line))
                continue;
            
            if (line[line.size()-1] == '\r')
                line = line.substr(0, line.size()-1);

            std::stringstream parser(line);
            MatVector<float> row;
            float temp;
            parser >> d;
            while (true) {
                parser >> temp;

                if (parser.fail()) {//try again, may be a separator
                    parser.clear();
                    char ch;
                    parser >> ch;
                    parser >> temp;
                }

                if (parser.eof()) {//Read group
                    row.append(d);
                    group = temp;
                    break;
                }
                
                if (!parser.bad()) {
                    row.append(d);
                    d = temp;
                }
            }
            
            dbgOut(3) << row.toString() << "\t" << group << endl;

            if (nColumns == -1)
                nColumns = row.size();
            else if (nColumns != row.size()) {
                file.close();
                dbgOut(0) << "Wrong number of columns in file " << filename << " at line: " << lineNumber << endl;
                dbgOut(1) << nColumns << "!=" << row.size() << endl << row.toString() << endl;
                return false;
            }

            
            bool itemFound = false;
            int itemIndex;
            for (std::map<int, int>::iterator it = groupLabels.begin(); it != groupLabels.end(); it++) {
                if (it->second == group) {
                    itemFound = true;
                    itemIndex = it->first;
                    break;
                }
            }

            if (!itemFound) {
                itemIndex = groupLabels.size();
                groupLabels[itemIndex] = group;
            }
            
            groups.push_back(group);

            data.concatRows(row);
        } while (true);
                
        file.close();

        return true;
    }
    
    static bool readArff(const std::string filename, MatMatrix<float> &data, std::map<int, int> &groupLabels, std::vector<int> &groups) {

        std::ifstream file(filename.c_str());

        if (!file.is_open()) {
            dbgOut(0) << "Could not open file:" << filename << endl;
            return false;
        }

        float d;
        int nColumns = -1;
        int group;
        std::string line;
        int lineNumber = 0;

        do {
            lineNumber++;
            getline(file, line, '\n');
           
            if (file.eof()) break;
            
            if (line.size() < 1 || line[0] == '@')
                continue;

            if (!hasDigits(line))
                continue;
            
            if (line[line.size()-1] == '\r')
                line = line.substr(0, line.size()-1);

            std::stringstream parser(line);
            MatVector<float> row;
            float temp;
            parser >> d;
            while (true) {
                parser >> temp;

                if (parser.fail()) {//try again, may be a separator
                    parser.clear();
                    char ch;
                    parser >> ch;
                    parser >> temp;
                }

                if (parser.eof()) {//Read group
                    row.append(d);
                    group = temp;
                    break;
                }
                
                if (!parser.bad()) {
                    row.append(d);
                    d = temp;
                }
            }
            
            dbgOut(3) << row.toString() << "\t" << group << endl;

            if (nColumns == -1)
                nColumns = row.size();
            else if (nColumns != row.size()) {
                file.close();
                dbgOut(0) << "Wrong number of columns in file " << filename << " at line: " << lineNumber << endl;
                dbgOut(1) << nColumns << "!=" << row.size() << endl << row.toString() << endl;
                return false;
            }

            
            bool itemFound = false;
            int itemIndex;
            for (std::map<int, int>::iterator it = groupLabels.begin(); it != groupLabels.end(); it++) {
                if (it->second == group) {
                    itemFound = true;
                    itemIndex = it->first;
                    break;
                }
            }

            if (!itemFound) {
                itemIndex = groupLabels.size();
                groupLabels[itemIndex] = group;
            }
            
            groups.push_back(itemIndex);

            data.concatRows(row);
        } while (true);
                
        file.close();

        return true;
    }
    
    static bool hasDigits(const std::string str) {
        for (int i=0; i<str.size(); i++)
            if (isdigit(str[i])) return true;
        return false;
    }
    
    static bool readArffD(const std::string filename, MatMatrix<float> &data, std::map<int, int> &groupLabels, std::vector<int> &groups) {

        std::ifstream file(filename.c_str());

        if (!file.is_open()) {
            dbgOut(0) << "Could not open file:" << filename << endl;
            return false;
        }

        float d;
        int nColumns = -1;
        int group;
        std::string line;
        int lineNumber = 0;

        do {
            lineNumber++;
            getline(file, line, '\n');
            if (line[0] == '@')
                continue;
            
            if (file.eof()) break;
            
            if (line[line.size()-1] == '\r')
                line = line.substr(0, line.size()-1);
            
            std::stringstream parser(line);
            MatVector<float> row;
            float temp;
            parser >> d;
            while (true) {
                parser >> temp;
                
                if (parser.fail()) {//try again, may be a separator
                    parser.clear();
                    char ch;
                    parser >> ch;
                    parser >> temp;
                }
                
                if (parser.eof()) {//Read group
                    row.append(d);
                    row.append(temp);
                    group = temp;
                    break;
                }
                
                if (!parser.bad()) {
                    row.append(d);
                    d = temp;
                }
            }
            
            dbgOut(2) << row.toString() << "\t" << group << endl;
            
            if (nColumns == -1)
                nColumns = row.size();
            else if (nColumns != row.size()) {
                file.close();
                dbgOut(0) << "Wrong number of columns in file " << filename << " at line: " << lineNumber << endl;
                return false;
            }
            
            bool itemFound = false;
            int itemIndex;
            for (std::map<int, int>::iterator it = groupLabels.begin(); it != groupLabels.end(); it++) {
                if (it->second == group) {
                    itemFound = true;
                    itemIndex = it->first;
                    break;
                }
            }

            if (!itemFound) {
                itemIndex = groupLabels.size();
                groupLabels[itemIndex] = group;
            }

            groups.push_back(itemIndex);

            data.concatRows(row);
        } while (true);
        file.close();

        return true;
    }

    static bool readArff(const std::string filename, std::vector<MatMatrix<float> *> &data, std::map<int, int> &groupLabels, std::vector<int> &groups, int &nColumns, bool useSeparator = true) {

        std::ifstream file(filename.c_str());

        if (!file.is_open()) {
            return false;
        }

        float d;
        nColumns = -1;
        int group;
        std::string line;

        int lineNumber = 0;
        do {
            lineNumber++;
            getline(file, line, '\n');
            if (line[0] == '@')
                continue;

            if (file.eof()) break;
            if (line[line.size()-1] == '\r')
                line = line.substr(0, line.size()-1);
            
            std::stringstream parser(line);
            MatMatrix<float> *row = new MatMatrix<float>;
            float temp;
            parser >> d;
            while (true) {
                if (parser >> temp) {
                    MatVector<float> colD(1);
                    colD[0] = d;
                    row->concatCols(colD);
                    d = temp;
                } else {//Read group
                    group = d;
                    break;
                }
            }

            if (nColumns == -1)
                nColumns = row->cols();
            else
                if (nColumns != row->cols()) {
                file.close();
                dbgOut(0) << "Wrong number of columns in file " << filename << " at line: " << lineNumber << endl;
                return false;
            }

            bool itemFound = false;
            int itemIndex;
            for (std::map<int, int>::iterator it = groupLabels.begin(); it != groupLabels.end(); it++) {
                if (it->second == group) {
                    itemFound = true;
                    itemIndex = it->first;
                    break;
                }
            }

            if (!itemFound) {
                itemIndex = groupLabels.size();
                groupLabels[itemIndex] = group;
            }

            groups.push_back(itemIndex);

            data.push_back(row);
        } while (true);
        file.close();

        return true;
    }

    static void rescaleCols01(MatMatrix<float> &data) {

        MatVector<float> datamin(data.cols());
        datamin.fill(std::numeric_limits<float>::max());
        MatVector<float> datamax(data.cols());
        datamax.fill(-std::numeric_limits<float>::max());

        for (int i = 0; i < data.rows(); i++) {
            for (int j = 0; j < data.cols(); j++) {
                if (data[i][j] > datamax[j]) datamax[j] = data[i][j];
                if (data[i][j] < datamin[j]) datamin[j] = data[i][j];
            }
        }

        for (int i = 0; i < data.rows(); i++) {
            for (int j = 0; j < data.cols(); j++) {
                if (datamax[j] - datamin[j] != 0)
                    data[i][j] = (data[i][j] - datamin[j]) / (datamax[j] - datamin[j]);
                else
                    data[i][j] = 0;
            }
        }
    }
    
    static void siftNormalize(MatMatrix<float> &data, float limit) {

        for (int i = 0; i < data.rows(); i++) {
            MatVector<float> row;
            data.getRow(i, row);
            //dbgOut(2) << row.toString() << endl;
            rescaleRowColsL2(row);
            for (int j = 0; j < row.size(); j++) {
                if (row[j]>limit) row[j] = limit;                    
            }
            row.mult(1/limit);
            data.setRow(i, row);
        }
    }
    
    static void rescaleRowCols01(MatVector<float> &row, MatVector<float> &datamin, MatVector<float> &datamax) {

        for (int j = 0; j < row.size(); j++) {
            if (datamax[j] - datamin[j] != 0)
                row[j] = (row[j] - datamin[j]) / (datamax[j] - datamin[j]);
            else
                row[j] = 0;
        }
    }
    
    static void rescaleRowColsL1(MatVector<float> &row) {

        float norm = row.sum();
        if (norm!=0)
        row = row/norm;
    }
    
    static void rescaleRowColsL2(MatVector<float> &row) {

        float norm = row.norm();
        if (norm!=0)
        row = row/norm;
    }
    
    static void rescaleRowColsMax(MatVector<float> &row) {

        float norm = row.max();
        if (norm!=0)
        row = row/norm;
    }
    
    static void rescaleRowColsMaxMin(MatVector<float> &row) {

        float max = row.max();
        float min = row.min();
        if ((max-min)!=0)
        row = (row-min)/(max-min);
    }

    static void rescaleCols01(std::vector<MatMatrix<float> *>&data) {

        MatVector<float> datamin(data[0]->cols());
        datamin.fill(std::numeric_limits<float>::max());
        MatVector<float> datamax(data[0]->cols());
        datamax.fill(-std::numeric_limits<float>::max());

        for (int d = 0; d < data.size(); d++) {
            MatMatrix<float> *mat = data[d];
            for (int j = 0; j < mat->cols(); j++) {
                if ((*mat)[0][j] > datamax[j]) datamax[j] = (*mat)[0][j];
                if ((*mat)[0][j] < datamin[j]) datamin[j] = (*mat)[0][j];
            }
        }

        for (int d = 0; d < data.size(); d++) {
            MatMatrix<float> *mat = data[d];
            for (int i = 0; i < mat->rows(); i++) {
                for (int j = 0; j < mat->cols(); j++) {
                    if (datamax[j] - datamin[j] != 0)
                        (*mat)[i][j] = ((*mat)[i][j] - datamin[j]) / (datamax[j] - datamin[j]);
                    else
                        (*mat)[i][j] = 0;
                }
            }
        }
    }

    static void getConfusionMatrix(const std::vector<int> &classes, const std::vector<int> &trueClasses, MatMatrix<int> &confusionMatrix) {

        confusionMatrix.size(classes.size(), trueClasses.size());
        confusionMatrix.fill(0);

        for (int i = 0; i < confusionMatrix.rows(); i++)
            for (int j = 0; j < confusionMatrix.cols(); j++)
                confusionMatrix[classes[i]][trueClasses[j]]++;
    }

    static void printConfusionMatrix(const MatMatrix<int> &cm, std::map<int, int> &groupLabels) {

        dbgOut(0) << "cluster\\class\t|";
        MatVector<int> rowSums(cm.rows());
        MatVector<int> colSums(cm.cols());
        rowSums.fill(0);
        colSums.fill(0);

        std::map<int, int>::iterator it;
        for (it = groupLabels.begin(); it != groupLabels.end(); it++)
            dbgOut(0) << "\tcla" << (*it).second;

        dbgOut(0) << "\t| Sum" << endl;
        for (int r = 0; r < cm.rows(); r++) {
            dbgOut(0) << "clu" << r << "\t\t|";
            for (int c = 0; c < cm.cols(); c++) {
                dbgOut(0) << "\t" << cm[r][c];
                rowSums[r] += cm[r][c];
                colSums[c] += cm[r][c];
            }
            dbgOut(0) << "\t| " << rowSums[r] << endl;
        }
        dbgOut(0) << "Sums\t\t|";
        for (int c = 0; c < cm.cols(); c++)
            dbgOut(0) << "\t" << colSums[c];
        dbgOut(0) << "\t| " << colSums.sum() << endl << endl;
    }

    static bool loadTruClusters(const std::string filename, MatMatrix<int> &truClusters) {
        std::ifstream file(filename.c_str());

        if (!file.is_open()) {
            return false;
        }

        int d;

        std::string line;
        do {
            getline(file, line, '\n');
            std::stringstream parser(line);
            {
                MatVector<int> row;
                while (parser >> d)
                    row.append(d);

                if (row.size() > 0)
                    truClusters.concatRows(row);
            }
        } while (!file.eof());
        file.close();

        return true;
    }
    
    static void getGroupAverages(const MatMatrix<float> &trainingData, const std::vector<int> &groups, const std::map<int,int> &groupLabels, MatMatrix<float> &averages) {
        averages.size(trainingData.size(), groupLabels.size());
        averages.fill(0);
        MatVector<int> count(groupLabels.size());
        count.fill(0);
    
        for (int i = 0; i < trainingData.rows(); i++) {
            for (int j = 0; j < trainingData.cols(); j++) {
                averages[j][groups[i]] += trainingData[i][j];
            }
            count[groups[i]]++;
        }

        for (int i = 0; i < groupLabels.size(); i++) {
            for (int j = 0; j < trainingData.cols(); j++) {
                averages[j][i] = averages[j][i] / (float) count[i];
            }
        }
    }
    
    static bool writeArff(std::string &filename, const MatMatrix<float> &data, const std::vector<int> &groups) {
       std::ofstream file(filename.c_str());
       
       std::map<int,int> groupLabels;
       
       for (int i=0; i<groups.size(); i++) {
           if (groupLabels.find(groups[i]) == groupLabels.end()) {
               groupLabels[groups[i]] = groups[i];
           }
       }
       
       if (!file.is_open()) {
           dbgOut(0) << "Error opening file: " << filename << endl;
           return false;
       }
       
       //Attribuites
       file << "@relation data" << endl;
       for (int i=0; i<data.cols(); i++) {
           file << "@attribute A" << i << " real" << endl;           
       }
       
       //Class
       file << "@attribute class {";
       int label;
       for (int i=0; i<groupLabels.size()-1; i++) { 
           label = groupLabels[i];
           file << label << ",";
       }
       file << groupLabels[groupLabels.size()-1];
       file << "}" << endl;
       
       //Data
       file << "@data" << endl;       
       for (int i=0; i<data.rows(); i++) {
           for (int j=0; j<data.cols(); j++) {
               file << data[i][j] << "\t";
           }
           file << groups[i] << endl;
       }
       
       file.close();
       return true;
    }
    
private:
    bool setRowValue(MatVector<float> &row, int col, float value) {
        if (col<row.size())
            row[col] = value;
        else if (col==row.size())
            row.append(value);
        else {
            return false;                        
        }
        
        return true;
    }    
};

#endif	/* ARFFDATA_H */
