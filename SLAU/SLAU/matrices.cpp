#include <iostream>
#include "matrices.h"
using namespace std;
double DegToRad(double D)
{
    double M = M_PI / 180;
    return D * M;
};

double** RotationMatrix(double x, double y, double z)
{
    double** matrixRot;
    double degX = DegToRad(x), degY = DegToRad(y), degZ = DegToRad(z);
    matrixRot = new double* [3];
    matrixRot[0] = new double[3];
    matrixRot[0][0] = cos(degZ) * cos(degY);
    matrixRot[0][1] = cos(degZ) * sin(degY) * sin(degX) - sin(degZ) * cos(degX);
    matrixRot[0][2] = cos(degZ) * sin(degY) * cos(degX) + sin(degZ) * sin(degX);

    matrixRot[1] = new double[3];
    matrixRot[1][0] = sin(degZ) * cos(degY);
    matrixRot[1][1] = sin(degZ) * sin(degY) * sin(degX) + cos(degZ) * cos(degX);
    matrixRot[1][2] = sin(degZ) * sin(degY) * cos(degX) - cos(degZ) * sin(degX);

    matrixRot[2] = new double[3];
    matrixRot[2][0] = -sin(degY);
    matrixRot[2][1] = cos(degY) * sin(degX);
    matrixRot[2][2] = cos(degY) * cos(degX);

    return matrixRot;
}

double** MainMatrix(double** rotataionMatrix1, double** rotataionMatrix2, double* vectorRes1, double* vectorRes2)
{
    double** mainMatrix = new double* [4];
    mainMatrix[0] = new double[3];
    mainMatrix[0][0] = rotataionMatrix1[0][0] - vectorRes1[0] * rotataionMatrix1[2][0];
    mainMatrix[0][1] = rotataionMatrix1[0][1] - vectorRes1[0] * rotataionMatrix1[2][1];
    mainMatrix[0][2] = rotataionMatrix1[0][2] - vectorRes1[0] * rotataionMatrix1[2][2];

    mainMatrix[1] = new double[3];
    mainMatrix[1][0] = rotataionMatrix1[1][0] - vectorRes1[0] * rotataionMatrix1[2][0];
    mainMatrix[1][1] = rotataionMatrix1[1][1] - vectorRes1[0] * rotataionMatrix1[2][1];
    mainMatrix[1][2] = rotataionMatrix1[1][2] - vectorRes1[0] * rotataionMatrix1[2][2];

    mainMatrix[2] = new double[3];
    mainMatrix[2][0] = rotataionMatrix2[0][0] - vectorRes2[0] * rotataionMatrix2[2][0];
    mainMatrix[2][1] = rotataionMatrix2[0][1] - vectorRes2[0] * rotataionMatrix2[2][1];
    mainMatrix[2][2] = rotataionMatrix2[0][2] - vectorRes2[0] * rotataionMatrix2[2][2];

    mainMatrix[3] = new double[3];
    mainMatrix[3][0] = rotataionMatrix2[1][0] - vectorRes2[0] * rotataionMatrix2[2][0];
    mainMatrix[3][1] = rotataionMatrix2[1][1] - vectorRes2[0] * rotataionMatrix2[2][1];
    mainMatrix[3][2] = rotataionMatrix2[1][2] - vectorRes2[0] * rotataionMatrix2[2][2];

    return mainMatrix;
}

double* MainResult(double* translationVector1, double* translationVector2, double* vectorRes1, double* vectorRes2)
{
    double* mainResult = new double[4];
    mainResult[0] = vectorRes1[0] - translationVector1[0];
    mainResult[1] = vectorRes1[1] - translationVector1[1];
    mainResult[2] = vectorRes2[0] - translationVector2[0];
    mainResult[3] = vectorRes2[1] - translationVector2[1];

    return mainResult;
}

double* gauss(double** a, double* y, int n)
{
    double* x, max;
    int k, index;
    const double eps = 0.0000001;  // точность
    x = new double[n];
    k = 0;
    while (k < n)
    {
        // Поиск строки с максимальным a[i][k]
        max = abs(a[k][k]);
        index = k;
        for (int i = k + 1; i < n; i++)
        {
            if (abs(a[i][k]) > max)
            {
                max = abs(a[i][k]);
                index = i;
            }
        }
        // Перестановка строк
        if (max < eps)
        {
            // нет ненулевых диагональных элементов
            cout << "Решение получить невозможно из-за нулевого столбца ";
            cout << index << " матрицы A" << endl;
            return 0;
        }
        for (int j = 0; j < n; j++)
        {
            double temp = a[k][j];
            a[k][j] = a[index][j];
            a[index][j] = temp;
        }
        double temp = y[k];
        y[k] = y[index];
        y[index] = temp;
        // Нормализация уравнений
        for (int i = k; i < n; i++)
        {
            double temp = a[i][k];
            if (abs(temp) < eps) continue; // для нулевого коэффициента пропустить
            for (int j = 0; j < n; j++)
                a[i][j] = a[i][j] / temp;
            y[i] = y[i] / temp;
            if (i == k)  continue; // уравнение не вычитать само из себя
            for (int j = 0; j < n; j++)
                a[i][j] = a[i][j] - a[k][j];
            y[i] = y[i] - y[k];
        }
        k++;
    }
    // обратная подстановка
    for (k = n - 1; k >= 0; k--)
    {
        x[k] = y[k];
        for (int i = 0; i < k; i++)
            y[i] = y[i] - a[i][k] * x[k];
    }
    return x;
}