/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DisplaySSHSOM.h
 * Author: Pedro Magalhaes
 *
 * Created on 8 de Fevereiro de 2018, 14:29
 */

#ifndef DISPLAYSSSOM_H
#define DISPLAYSSSOM_H

#include <stdlib.h>
#include <fstream>
#include "SSSOM.h"
#include "DisplayMap.h"

class DisplaySSSOM: public DisplayMap {

    SSSOM *som;

public:
    
    DisplaySSSOM(SSSOM *som, MatMatrix<float> *trainingData, MatMatrix<float> *averages = NULL, map<int, int> *groupLabels = NULL, int padding = 20, int gitter = 0, bool bmucolor = true, bool trueClustersColor = true, bool filterNoise = false):DisplayMap(trainingData, averages, groupLabels, padding, gitter, bmucolor, trueClustersColor, filterNoise) {
        this->som = som;
    }
    
    virtual void getColorIndex(MatVector<float> &dataVector, int &index, int &size) {
        som->enumerateNodes();
        size = som->meshNodeSet.size();
        SSSOMNode *bmu = som->getWinner(dataVector);
        index = bmu->getId();
    }
    
    virtual void getClassIndex(MatVector<float> &dataVector, int &index, int &size) {
        if (groupLabels!=NULL) {
            size = groupLabels->size();
        }
        SSSOMNode *bmu = som->getWinner(dataVector);
        index = bmu->cls;
    }

    virtual void plotMap(CImg<unsigned char> *image, bool drawNodes, bool drawConections) {
        
        int nclass = 2;
        if (groupLabels!=NULL) {
            nclass = groupLabels->size();
        }
        
        unsigned char bmuColor[3];
        unsigned char contour[3];
        
        int width = (image->width() - 2 * padding);
        int height = (image->height() - 2 * padding);

        if (drawNodes) {
            MatMatrix<float> centers;
            som->outputCenters(centers);
            for (SSSOMNode *bmu = som->getFirstNode(); !som->finished(); bmu = som->getNextNode()) {
                int r, g, b;
                int size = som->size()-1;
                if (size==0) size = 1;
                int h = HUE_START + bmu->getId()*MAX_HUE / (size);
                HSVtoRGB(&r, &g, &b, h, 255, 255);
                bmuColor[0] = r;
                bmuColor[1] = g;
                bmuColor[2] = b;
                
                if (bmu->cls!=som->noCls) {
                    h = HUE_START + bmu->cls*MAX_HUE / (nclass);
                    HSVtoRGB(&r, &g, &b, h, 255, 255);
                    contour[0] = r;
                    contour[1] = g;
                    contour[2] = b;
                } else {
                    contour[0] = 0;
                    contour[1] = 0;
                    contour[2] = 0;
                }

                image->draw_circle(padding + bmu->w[X] * width, padding + bmu->w[Y] * height, 4, contour);
                image->draw_circle(padding + bmu->w[X] * width, padding + bmu->w[Y] * height, 3, bmuColor);
                int cx = bmu->w[X] * width;
                int cy = bmu->w[Y] * height;
                int x0 = bmu->w[X] * width - bmu->ds[X]*20;
                int x1 = bmu->w[X] * width + bmu->ds[X]*20;
                int y0 = bmu->w[Y] * height - bmu->ds[Y]*20;
                int y1 = bmu->w[Y] * height + bmu->ds[Y]*20;
                image->draw_line(padding + cx, padding + y0, padding + cx, padding + y1, contour);
                image->draw_line(padding + x0, padding + cy, padding + x1, padding + cy, contour);
            }
            
            //Draw connections
            if (drawConnections) {
                SSSOM::TPConnectionSet::iterator it;
                for (it = som->meshConnectionSet.begin(); it != som->meshConnectionSet.end(); it++) {
                    float x0 = (*it)->node[0]->w[X];
                    float y0 = (*it)->node[0]->w[Y];
                    float x1 = (*it)->node[1]->w[X];
                    float y1 = (*it)->node[1]->w[Y];
                    float dist = (*it)->node[0]->ds.dist((*it)->node[1]->ds) / sqrt((*it)->node[1]->ds.size());
                    unsigned char conColor[] = {255, 255, 255};
                    conColor[0] = conColor[0] * dist;
                    conColor[1] = conColor[1] * dist;
                    conColor[2] = conColor[2] * dist;
                    image->draw_line(padding + x0 * width, padding + y0 * height, padding + x1 * width, padding + y1 * height, conColor);
                }
            }
        }
    }
};

#endif /* DISPLAYSSSOM_H */

