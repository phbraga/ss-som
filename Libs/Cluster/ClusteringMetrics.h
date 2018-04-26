/* 
 * File:   ClusteringMetrics.h
 * Author: hans
 *
 * Created on 30 de Março de 2012, 11:29
 */

#ifndef CLUSTERINGMETRICS_H
#define	CLUSTERINGMETRICS_H

#include "MatMatrix.h"

class ClusteringMetrics {
public:

    static double RANDI(MatMatrix<int> &matrix) {
        double a = 0;
        for (int i = 0; i < matrix.rows(); i++)
            for (int j = 0; j < matrix.cols(); j++)
                a += (matrix[i][j]*(matrix[i][j] - 1)) / 2;

        double b = 0;
        double sum;
        for (int i = 0; i < matrix.rows(); i++) {
            sum = 0;
            for (int j = 0; j < matrix.cols(); j++)
                sum += matrix[i][j];
            b += (sum * (sum - 1)) / 2;
        }
        b = b - a;

        double c = 0;
        for (int i = 0; i < matrix.cols(); i++) {
            sum = 0;
            for (int j = 0; j < matrix.rows(); j++)
                sum += matrix[j][i];
            c += (sum * (sum - 1)) / 2;
        }
        c = c - a;

        double d = 0;
        sum = 0;
        for (int i = 0; i < matrix.rows(); i++)
            for (int j = 0; j < matrix.cols(); j++)
                sum += matrix[i][j];

        d = ((sum * (sum - 1)) / 2) - a - b - c;

        return (a + d) / (a + b + c + d);
    }

    static double ARI(MatMatrix<int> &matrix) {
        double a = 0;
        for (int i = 0; i < matrix.rows(); i++)
            for (int j = 0; j < matrix.cols(); j++)
                a += (matrix[i][j]*(matrix[i][j] - 1)) / 2;

        double b = 0;
        double sum;
        for (int i = 0; i < matrix.rows(); i++) {
            sum = 0;
            for (int j = 0; j < matrix.cols(); j++)
                sum += matrix[i][j];
            b += (sum * (sum - 1)) / 2;
        }

        double c = 0;
        for (int i = 0; i < matrix.cols(); i++) {
            sum = 0;
            for (int j = 0; j < matrix.rows(); j++)
                sum += matrix[j][i];
            c += (sum * (sum - 1)) / 2;
        }

        double d = 0;
        sum = 0;
        for (int i = 0; i < matrix.rows(); i++)
            for (int j = 0; j < matrix.cols(); j++)
                sum += matrix[i][j];

        d = ((sum * (sum - 1)) / 2);

        double n = 0;
        sum = 0;
        for (int i = 0; i < matrix.rows(); i++)
            for (int j = 0; j < matrix.cols(); j++)
                sum += matrix[i][j];

        n = (sum * (sum - 1)) / 2;

        double num = a - (c * b) / n;
        double den = ((c + b) / 2) - ((b * c) / n);
        return num / den;
    }

private:

};

#endif	/* CLUSTERINGMETRICS_H */

