/* 
 * File:   DisplayMap.h
 * Author: hans
 *
 * Created on 29 de Março de 2012, 08:56
 */

#ifndef DISPLAYMAP_H
#define	DISPLAYMAP_H

#include <sstream>
#include <time.h>
#include "CImg.h"
#include "DebugOut.h"
#include "MatMatrix.h"
#include <vector>
#include <map>

using namespace cimg_library;
using namespace std;

const unsigned char contour[] = {0, 0, 0};
const unsigned char background[] = {255, 255, 255};

#define KEY_SLEEP 100
#define HUE_START 50
#define MAX_HUE 300

class DisplayMap {
protected:

    CImg<unsigned char> *image;
    CImgDisplay *disp;
    MatMatrix<float> *trainingData;
    std::vector<std::vector<int> > trueClusters;
    
    int X;
    int Y;
    bool drawNodes;
    bool bmucolor;
    bool trueClustersColor;
    bool drawConnections;
    bool filterNoise;
    bool pause;
    int padding;
    int gitter;
    MatMatrix<float> *averages;
    map<int, int> *groupLabels;
    int maxDataPlot;
    
    clock_t now;
    clock_t last;

public:

    DisplayMap(MatMatrix<float> *trainingData, MatMatrix<float> *averages = NULL, map<int, int> *groupLabels = NULL, int padding = 20, int gitter = 0, bool bmucolor = true, bool trueClustersColor = true, bool filterNoise = false) {

        this->trainingData = trainingData;
        setAverages(averages);
        setGroupLabels(groupLabels);
        setBmucolor(bmucolor);
        setPadding(padding);
        setGitter(gitter);
        setTrueClustersColor(trueClustersColor);
        setDrawNodes(true);
        setDrawConnections(true);
        setFilterNoise(filterNoise);
        pause = false;
        now = last = 0;
        maxDataPlot = -1;

        image = new CImg<unsigned char>(500, 500, 1, 3);
        disp = new CImgDisplay(500, 500);

        X = 0;
        Y = 1;
    }

    void setGroupLabels(map<int, int>* groupLabels) {
        this->groupLabels = groupLabels;
    }

    void setAverages(MatMatrix<float>* averages) {
        this->averages = averages;
    }
    
    void setTrainingData(MatMatrix<float>* trainingData) {
        this->trainingData = trainingData;
    }

    void setDrawNodes(bool drawNodes) {
        this->drawNodes = drawNodes;
    }
    
    void setBmucolor(bool bmucolor) {
        this->bmucolor = bmucolor;
    }

    void setTrueClustersColor(bool trueClustersColor) {
        this->trueClustersColor = trueClustersColor;
    }
    
    void setFilterNoise(bool filterNoise) {
        this->filterNoise = filterNoise;
    }

    void setGitter(int gitter) {
        this->gitter = gitter;
    }

    void setPadding(int padding) {
        this->padding = padding;
    }
    
    void setMaxDataPlot(int max) {
        this->maxDataPlot = max;
    }

    void setY(int Y) {
        this->Y = Y;
    }

    void setX(int X) {
        this->X = X;
    }

    void setDrawConnections(bool drawConnections) {
        this->drawConnections = drawConnections;
    }

    bool isDrawConnections() const {
        return drawConnections;
    }

    ~DisplayMap() {
        delete image;
        delete disp;
    }

    void updateTitle() {
        stringstream str;
        str << "View:" << X << "X" << Y;
        if (trueClustersColor) 
            str << " - True Clusters Colors";
        if (filterNoise)
            str << " - Filter Noise";
        
        disp->set_title(str.str().c_str());
    }

    void display(MatVector<float> *data = NULL) {

        processKey();
        updateTitle();
        if (!pause && !disp->is_closed()) {
            buildImage(X, Y, data);
            plotMap(image, drawNodes, drawConnections);
            image->display(*disp);
        }
    }

    void display(int X, int Y, MatVector<float> *data = NULL) {
        updateTitle();

        if (!pause && !disp->is_closed()) {
            buildImage(X, Y, data);
            plotMap(image, drawNodes, drawConnections);
            image->display(*disp);
        }
    }

    void displayLoop(MatVector<float> *data = NULL) {

        while (!disp->is_closed()) {
            processKey();

            updateTitle();

            if (!pause) {
                buildImage(X, Y, data);
                plotMap(image, drawNodes, drawConnections);
                image->display(*disp);
            }
            //disp->wait();
        }
    }

    void close() {
        disp->close();
    }

    void setTrueClustersData(std::vector<std::vector<int> > &trueClusters) {
        this->trueClusters = trueClusters;
    }

    void setTrueClustersData(std::vector<int> &groups) {
        trueClusters.clear();
        std::map<int, std::vector<int> > gm;
        
        for (int i=0; i<groups.size(); i++) {
            gm[groups[i]].push_back(i);
            //dbgOut(0) << "cluster " << groups[i] << " size: " << gm[groups[i]].size() << endl; 
        }
        
        for (int i=0; i<gm.size(); i++) {
            trueClusters.push_back(gm[i]);
        }
    }

    void loadTrueClustersData(std::string filename) {

        trueClusters.clear();
        string line;

        //Read true clusters
        int period = filename.find_last_of(".");
        string trueClustersFileName = filename.substr(0, period) + ".true";
        ifstream trueFile(trueClustersFileName.c_str());

        if (!trueFile.is_open()) {
            dbgOut(1) << "Error reading true clusters file:" << trueClustersFileName << endl;
            return;
        }

        trueFile.ignore(4); //Skip "DIM="
        int dim;
        trueFile >> dim; //Read number of dimensions
        getline(trueFile, line); //Skip first line

        int dimvalues;
        int clusterSize;
        int objectIndex;
        int clust = 0;
        while (true) {
            for (int i = 0; i < dim; i++) {//Skip dimension values
                trueFile >> dimvalues;
                if (trueFile.eof())
                    break;
            }
            if (trueFile.eof()) break;

            trueFile >> clusterSize;

            std::vector<int> cluster;
            for (int i = 0; i < clusterSize; i++) {
                trueFile >> objectIndex;
                if (trueFile.eof()) break;
                cluster.push_back(objectIndex);
            }
            trueClusters.push_back(cluster);
            clust++;
        }
        trueFile.close();
    }

    void setPause(bool pause) {
        this->pause = pause;
    }

    bool isPaused() const {
        return pause;
    }

private:

    void processKey() {
        now = clock() / (CLOCKS_PER_SEC / 1000);
        
        if (now - last>KEY_SLEEP) {
            
            if (disp->key() == cimg::keyP) {
                pause = !pause;
            }
                        
            if (disp->key() == cimg::keyARROWUP) {
                Y++;
                if (Y >= trainingData->cols())
                    Y = 0;
            }

            if (disp->key() == cimg::keyARROWDOWN) {
                Y--;
                if (Y < 0)
                    Y = trainingData->cols() - 1;
            }

            if (disp->key() == cimg::keyARROWRIGHT) {
                X++;
                if (X >= trainingData->cols())
                    X = 0;
            }

            if (disp->key() == cimg::keyARROWLEFT) {
                X--;
                if (X < 0)
                    X = trainingData->cols() - 1;
            }

            if (disp->key() == cimg::keyESC) {
                disp->close();
            }

            if (disp->key() == cimg::keyU) {
                drawNodes = !drawNodes;
            }
            
            if (disp->key() == cimg::keyC) {
                drawConnections = !drawConnections;
            }

            if (disp->key() == cimg::keyT) {
                trueClustersColor = !trueClustersColor;
            }

            if (disp->key() == cimg::keyN) {
                filterNoise = !filterNoise;
            }

            if (disp->key() == cimg::keyB) {
                bmucolor = !bmucolor;
            }            

            if (disp->key() == cimg::keyPADADD) {
                gitter+=5;
            }

            if (disp->key() == cimg::keyPADSUB) {
                if (gitter>0)
                    gitter-=5;   
            }

            last = now;
        }
    }
    
    virtual bool isNoise(MatVector<float> &dataVector){
        
    }
    
    virtual void getColorIndex(MatVector<float> &dataVector, int &index, int &size) {
        index = 1;
        size = 1;
    }
    
    virtual void getClassIndex(MatVector<float> &dataVector, int &index, int &size) {
        index = 1;
        size = 1;
    }
        
    virtual void plotMap(CImg<unsigned char> *image, bool drawNodes, bool drawConections) {
    }

    virtual void buildImage(int X, int Y, MatVector<float> *dataVector, bool clean = true) {

        unsigned char bmuColor[3];
        unsigned char contourColor[3];
        
        int width = (image->width() - 2 * padding);
        int height = (image->height() - 2 * padding);

        if (clean) image->fill(background[0], background[1], background[2]);

        //Draw data
        if (trueClustersColor && trueClusters.size() > 0)
            drawTrueClusters(X, Y);
        else{
            int size = trainingData->rows()-1;
            if (maxDataPlot>0)
                size = maxDataPlot;
                
            for (int k = 0; k < size; k++) {
                int i = k;
                if (maxDataPlot>0)
                        i = rand()%size;

                MatVector<float> row;
                trainingData->getRow(i, row);
                if (filterNoise && isNoise(row))
                    continue;
                
                int gitterx = applyGitter(gitter);
                int gittery = applyGitter(gitter);

                if (bmucolor) {
                   
                    int r, g, b, index, size;
                    getColorIndex(row, index, size);
                    if (size==0) size = 1;
                    int h = HUE_START + index*MAX_HUE / (size);
                    HSVtoRGB(&r, &g, &b, h, 255, 255);
                    bmuColor[0] = r;
                    bmuColor[1] = g;
                    bmuColor[2] = b;
                    
                    getClassIndex(row, index, size);
                    if (size==0) size = 1;
                    h = HUE_START + index*MAX_HUE / (size);
                    HSVtoRGB(&r, &g, &b, h, 255, 255);
                    contourColor[0] = r;
                    contourColor[1] = g;
                    contourColor[2] = b;

                    image->draw_circle(padding + (*trainingData)[i][X] * width + gitterx, padding + (*trainingData)[i][Y] * height + gittery, 3, contourColor);
                    image->draw_circle(padding + (*trainingData)[i][X] * width + gitterx, padding + (*trainingData)[i][Y] * height + gittery, 2, bmuColor);
                }
                else
                    image->draw_circle(padding + (*trainingData)[i][X] * width + gitterx, padding + (*trainingData)[i][Y] * height + gittery, 2, contour);
            }
        }

        //draw labels
        if (averages != NULL) {
            for (int labelIndex = 0; labelIndex < averages->cols(); labelIndex++) {
                std::stringstream sstr;
                sstr << (*groupLabels)[labelIndex];
                float x = averages->get(X, labelIndex);
                float y = averages->get(Y, labelIndex);
                image->draw_text(padding + x * width, padding + y * height, sstr.str().c_str(), contour);
            }
        }
    }

    void drawTrueClusters(int X, int Y) {
        unsigned char color[3];
        unsigned char contourColor[3];
        int r, g, b;
        int width = (image->width() - 2 * padding);
        int height = (image->height() - 2 * padding);

        int size = trueClusters.size();
        if (maxDataPlot>0)
            size = maxDataPlot;
        
        int start = 0, end = size, inc = 1;
        
        //swich class print order in 50% of the time
        if (rand()%2==0) {
            start = size-1; end = -1; inc = -1;
        }
        
        for (int k = start; k != end; k+=inc) {
            int c = k;
            if (maxDataPlot>0)
                c = rand()%size;
            
            int h = HUE_START + c * MAX_HUE / (size);
            HSVtoRGB(&r, &g, &b, h, 255, 255);
            color[0] = r;
            color[1] = g;
            color[2] = b;

            std::vector<int> &indices = trueClusters[c];
            for (int i = 0; i < indices.size(); i++) {
                int gitterx = applyGitter(gitter);
                int gittery = applyGitter(gitter);

                MatVector<float> sample;
                trainingData->getRow(indices[i], sample);
                
                int index, size;
                getClassIndex(sample, index, size);
                if (size==0) size = 1;
                h = HUE_START + index*MAX_HUE / (size);
                HSVtoRGB(&r, &g, &b, h, 255, 255);
                contourColor[0] = r;
                contourColor[1] = g;
                contourColor[2] = b;

                image->draw_circle(padding + sample[X] * width + gitterx, padding + sample[Y] * height + gittery, 3, contourColor);
                image->draw_circle(padding + sample[X] * width + gitterx, padding + sample[Y] * height + gittery, 2, color);
            }
        }
    }

public:
    
    int applyGitter(int gitter) {

        if (gitter==0)
            return 0;
        
        float r = ((float) rand() / (RAND_MAX - 1));
        return (gitter * r - gitter / 2);
    }

    void HSVtoRGB(int *r, int *g, int *b, int h, int s, int v) {
        int f;
        long p, q, t;

        h = h%360;
        
        if (s == 0) {
            *r = *g = *b = v;
            return;
        }

        f = ((h % 60)*255) / 60;
        h /= 60;

        p = (v * (256 - s)) / 256;
        q = (v * (256 - (s * f) / 256)) / 256;
        t = (v * (256 - (s * (256 - f)) / 256)) / 256;

        switch (h) {
            case 0:
                *r = v;
                *g = t;
                *b = p;
                break;
            case 1:
                *r = q;
                *g = v;
                *b = p;
                break;
            case 2:
                *r = p;
                *g = v;
                *b = t;
                break;
            case 3:
                *r = p;
                *g = q;
                *b = v;
                break;
            case 4:
                *r = t;
                *g = p;
                *b = v;
                break;
            default:
                *r = v;
                *g = p;
                *b = q;
                break;
        }
    }

};

#endif	/* DISPLAYMAP_H */

