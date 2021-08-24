#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

double DegToRad(double D);

double** RotationMatrix(double x, double y, double z);

double** MainMatrix(double** rotataionMatrix1, double** rotataionMatrix2, double* vectorRes1, double* vectorRes2);

double* MainResult(double* translationVector1, double* translationVector2, double* vectorRes1, double* vectorRes2);

double* gauss(double** a, double* y, int n);