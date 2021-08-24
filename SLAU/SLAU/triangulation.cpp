#include"triangulation.h"

double Triangulation::DegToRad(double D)
{
    double M = M_PI / 180;
    return D * M;
};

vector<vector <double>> Triangulation::RotationMatrix(double x, double y, double z)
{
    vector<vector <double>> matrixRot;
    double degX = DegToRad(x), degY = DegToRad(y), degZ = DegToRad(z);

    matrixRot.push_back(vector<double>{cos(degZ)* cos(degY),
                                       cos(degZ)* sin(degY)* sin(degX) - sin(degZ) * cos(degX),
                                       cos(degZ)* sin(degY)* cos(degX) + sin(degZ) * sin(degX)});

    matrixRot.push_back(vector<double>{sin(degZ)* cos(degY),
                                       sin(degZ)* sin(degY)* sin(degX) + cos(degZ) * cos(degX),
                                       sin(degZ)* sin(degY)* cos(degX) - cos(degZ) * sin(degX)});

    matrixRot.push_back(vector<double>{-sin(degY),
                                        cos(degY)* sin(degX),
                                        cos(degY)* cos(degX)});
    return matrixRot;
}

vector<vector <double>> Triangulation::MainMatrix(vector<vector <double>> rotataionMatrix1, vector<vector <double>> rotataionMatrix2, vector<double> vectorRes1, vector<double> vectorRes2)
{
    vector<vector <double>> mainMatrix;
    mainMatrix.push_back(vector <double>{rotataionMatrix1[0][0] - vectorRes1[0] * rotataionMatrix1[2][0],
                                         rotataionMatrix1[0][1] - vectorRes1[0] * rotataionMatrix1[2][1],
                                         rotataionMatrix1[0][2] - vectorRes1[0] * rotataionMatrix1[2][2]});

    mainMatrix.push_back(vector <double>{rotataionMatrix1[1][0] - vectorRes1[0] * rotataionMatrix1[2][0],
                                         rotataionMatrix1[1][1] - vectorRes1[0] * rotataionMatrix1[2][1],
                                         rotataionMatrix1[1][2] - vectorRes1[0] * rotataionMatrix1[2][2] });

    mainMatrix.push_back(vector <double>{rotataionMatrix2[0][0] - vectorRes2[0] * rotataionMatrix2[2][0],
                                         rotataionMatrix2[0][1] - vectorRes2[0] * rotataionMatrix2[2][1],
                                         rotataionMatrix2[0][2] - vectorRes2[0] * rotataionMatrix2[2][2]});

    mainMatrix.push_back(vector <double>{rotataionMatrix2[1][0] - vectorRes2[0] * rotataionMatrix2[2][0],
                                         rotataionMatrix2[1][1] - vectorRes2[0] * rotataionMatrix2[2][1],
                                         rotataionMatrix2[1][2] - vectorRes2[0] * rotataionMatrix2[2][2]});
    return mainMatrix;
}

vector<double> Triangulation::MainResult(vector<double> translationVector1, vector<double> translationVector2, vector<double> vectorRes1, vector<double> vectorRes2)
{
    vector<double> mainResult;
    mainResult.push_back(vectorRes1[0] - translationVector1[0]);
    mainResult.push_back(vectorRes1[1] - translationVector1[1]);
    mainResult.push_back(vectorRes2[0] - translationVector2[0]);
    mainResult.push_back(vectorRes2[1] - translationVector2[1]);

    return mainResult;
}

vector<double> Triangulation::gauss(vector<vector <double>> a, vector<double> y, int n)
{
    double  max;
    vector<double> x;
    int k, index;
    const double eps = 0.0000001;  // точность
    x.resize(n);
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
            return vector<double>{0};
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

Triangulation::Triangulation(vector<double> dataFirstCam, vector<double> dataSecondCam)
{
    this->dataFirstCam = dataFirstCam;
    this->dataSecondCam = dataSecondCam;
}

vector<double> Triangulation::triangulation()
{
    vector <double> translationVector1 = { dataFirstCam[0], dataFirstCam[1], dataFirstCam[2] }, 
                    translationVector2 = { dataSecondCam[0], dataSecondCam[1], dataSecondCam[2] };

    vector <double> vectorRes1 = { dataFirstCam[3], dataFirstCam[4] }, 
                    vectorRes2 = { dataSecondCam[3], dataSecondCam[4] };

    vector<vector <double>> rotataionMatrix1 = RotationMatrix(dataFirstCam[5], 0, dataFirstCam[6]), rotationMatrix2 = RotationMatrix(dataSecondCam[5], 0, dataSecondCam[6]);
    
    vector<vector <double>> mainMatrix = MainMatrix(rotataionMatrix1, rotationMatrix2, vectorRes1, vectorRes2);

    vector <double> mainResult = MainResult(translationVector1, translationVector2, vectorRes1, vectorRes2);

    vector<double> result = gauss(mainMatrix, mainResult, 3);

    return result;
}