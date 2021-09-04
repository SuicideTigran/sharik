#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#include <iostream>
#include <QVector>
#include <QGeoCoordinate>
#include <math.h>

enum indexDataCam {lat, lon, hgt, azim, elevation, resolutionW, resolutionH,
     positionX, positionY, sizeW, sizeH, fowW, fowH};
enum indexVector {dirX, dirY, dirZ};

QVector<double> gauss(QVector<QVector <double>> a, QVector<double> y, int n);

double degToRad(double D);

bool checkGauss(QVector<QVector <double>> matrix, QVector <double> vec);

QVector <double> calculateResult(QVector <double> dataFirstCam,QVector <double> dataSecondCam);

bool equalVector(QVector <double> firstVec, QVector <double> secondVec, double eps);

QVector <double> kramer(QVector<QVector <double>> matrix, QVector <double> vec);

double det (QVector<QVector <double>> mtrx);

#endif // MATHEMATICS_H
