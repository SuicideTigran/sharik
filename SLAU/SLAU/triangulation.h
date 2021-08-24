#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;


class Triangulation
{
public:
	Triangulation(vector<double> dataFirstCam, vector<double> dataSecondCam);

	vector<double> triangulation();

private:
	double DegToRad(double D);

	vector<vector <double>> RotationMatrix(double x, double y, double z);

	vector<vector <double>> MainMatrix(vector<vector <double>> rotataionMatrix1, vector<vector <double>> rotataionMatrix2, vector<double> vectorRes1, vector<double> vectorRes2);

	vector<double> MainResult(vector<double> translationVector1, vector<double> translationVector2, vector<double> vectorRes1, vector<double> vectorRes2);

	vector<double> gauss(vector<vector <double>> a, vector<double> y, int n);

private:
	vector<double> dataFirstCam, dataSecondCam;
};