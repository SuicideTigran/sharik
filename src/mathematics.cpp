#include "mathematics.h"

#include <QtMath>

namespace {
    enum {vecX, vecY, vecZ};
}

namespace shmath {

    double degToRad(double D)
    {
        double M = M_PI / 180;
        return D * M;
    };

    //Проверка на существование решения
    bool checkKramer(QVector<QVector <double>> matrix, QVector <double> vec)
    {
        QVector <QVector <double>> matFirst, matThird;
        QVector <double> vecFirst, vecThird;
        for(int i = 0; i < matrix.size() - 1; ++i)
        {
            QVector <double> rowFirst, rowThird;
            for(int j = 0; j < matrix[i].size(); ++j)
            {
                rowFirst.push_back(matrix[i][j]);
                rowThird.push_back(matrix[(i+2)%4][j]);
            }
            matFirst.push_back(rowFirst);
            matThird.push_back(rowThird);

            vecFirst.push_back(vec[i]);
            vecThird.push_back(vec[(i+2)%4]);
        }

        QVector <double> resFirst = kramer(matFirst, vecFirst), resThird = kramer(matThird, vecThird);
        double eps = 0;
        for(int i = 0; i < resFirst.size(); ++i)
        {
            eps += (resFirst[i] - resThird[i]) * (resFirst[i] - resThird[i]);
        }
        //qDebug() << eps;
        return (eps < 1);
    }

    QVector <double> calculateResult(QVector <double> dataFirstCam,QVector <double> dataSecondCam)
    {
        QGeoCoordinate firstCam(dataFirstCam[lat], dataFirstCam[lon]), secondCam(dataSecondCam[lat], dataSecondCam[lon]);
        double distance = firstCam.distanceTo(secondCam);
        double azimuth = firstCam.azimuthTo(secondCam);

        //Перевод в декартову систему координат
        double firstCamX = 0, firstCamY = 0, firstCamZ = 0;
        double secondCamX = distance * sin(degToRad(azimuth));
        double secondCamY = distance * cos(degToRad(azimuth));
        double secondCamZ = dataSecondCam[hgt] - dataFirstCam[hgt];

        //qDebug() << "Frist Camera:" << firstCamX << "  " << firstCamY << "  " << firstCamZ;
        //qDebug() << "Second Camera:" << secondCamX << "  " << secondCamY << "  " << secondCamZ;

        double azimuthFirstCam = dataFirstCam[azim], elevationFirstCam = dataFirstCam[elevation], azimuthSecondCam = dataSecondCam[azim], elevationSecondCam = dataSecondCam[elevation];
        double fowWidthFirstCam = dataFirstCam[fowW], fowHeightFirstCam = dataFirstCam[fowH], fowWidthSecondCam = dataSecondCam[fowW], fowHeightSecondCam = dataSecondCam[fowH];
        double resolutionWidthFirstCam = dataFirstCam[resolutionW], resolutionHeightFirstCam = dataFirstCam[resolutionH], resolutionWidthSecondCam = dataSecondCam[resolutionW], resolutionHeightSecondCam = dataSecondCam[resolutionH];
        double positionXFirstCam = dataFirstCam[positionX], positionYFirstCam = dataFirstCam[positionY], positionXSecondCam = dataSecondCam[positionX], positionYSecondCam = dataSecondCam[positionY];

        //Сдвиг от центрального луча
        double aFirstCam = fowWidthFirstCam / resolutionWidthFirstCam, bFirstCam = fowHeightFirstCam / resolutionHeightFirstCam,
               aSecondCam = fowWidthSecondCam / resolutionWidthSecondCam, bSecondCam = fowHeightSecondCam / resolutionHeightSecondCam;
        double alphaFirstCam = (azimuthFirstCam + aFirstCam * positionXFirstCam - fowWidthFirstCam / 2),
               betaFirstCam = (elevationFirstCam - bFirstCam * positionYFirstCam + fowHeightFirstCam / 2);
        double alphaSecondCam = (azimuthSecondCam + aSecondCam * positionXSecondCam - fowWidthSecondCam / 2),
               betaSecondCam = (elevationSecondCam - bSecondCam * positionYSecondCam + fowHeightSecondCam / 2);

        //Вектор направления
        QVector <double> vecFirstCam = {cos(degToRad(betaFirstCam))*sin(degToRad(alphaFirstCam)), cos(degToRad(alphaFirstCam))*cos(degToRad(betaFirstCam)), sin(degToRad(betaFirstCam))},
                         vecSecondCam = {cos(degToRad(betaSecondCam))*sin(degToRad(alphaSecondCam)), cos(degToRad(alphaSecondCam))*cos(degToRad(betaSecondCam)), sin(degToRad(betaSecondCam))};

        //СЛУ из уравнений двух прямых
        QVector <QVector <double>> matrix;
        matrix.push_back(QVector <double> {vecFirstCam[dirY], -vecFirstCam[dirX], 0});
        matrix.push_back(QVector <double> {0, -vecFirstCam[dirZ], vecFirstCam[dirY]});
        matrix.push_back(QVector <double> {vecSecondCam[dirY], -vecSecondCam[dirX], 0});
        matrix.push_back(QVector <double> {0, -vecSecondCam[dirZ], vecSecondCam[dirY]});

        QVector <double> rVec = {vecFirstCam[dirY] * firstCamX - vecFirstCam[dirX] * firstCamY,
        vecFirstCam[dirY] * firstCamZ - vecFirstCam[dirZ] * firstCamY,
        vecSecondCam[dirY] * secondCamX - vecSecondCam[dirX] * secondCamY,
        vecSecondCam[dirY] * secondCamZ - vecSecondCam[dirZ] * secondCamY};

        if(checkKramer(matrix, rVec))
        {
            QVector <double> resultDec = kramer(matrix, rVec);

            //Перевод в географические координаты
            double distanceRes = sqrt(resultDec[dirX]*resultDec[dirX] + resultDec[dirY]*resultDec[dirY]);
            QGeoCoordinate resultGeoCoord = firstCam.atDistanceAndAzimuth(distanceRes, alphaFirstCam);
            //qDebug() << "GeoResult:  " << resultGeoCoord.latitude() << "  " << resultGeoCoord.longitude() << "  " << resultDec[2] + dataFirstCam[2];

            //Поиск высоты и ширины объекта
            double angleWidthFirstBall = dataFirstCam[sizeW], angleHeightFirstBall = dataFirstCam[sizeH];
            double distanceToFirstBall =  sqrt((resultDec[dirX]) * (resultDec[dirX]) + (resultDec[dirY])*(resultDec[dirY]) + (resultDec[dirZ]) * (resultDec[dirZ]) );
            double disFirstCenter = distanceToFirstBall * cos( degToRad(- bFirstCam * positionYFirstCam + fowHeightFirstCam / 2)) * cos( degToRad(aFirstCam * positionXFirstCam - fowWidthFirstCam / 2));

            double totalFirstH = 2 * disFirstCenter * tan(degToRad(fowHeightFirstCam/2));
            double sizeFirstH = totalFirstH * angleHeightFirstBall / resolutionHeightFirstCam;

            double totalFirstW = 2 * disFirstCenter * tan(degToRad(fowWidthFirstCam/2));
            double sizeFirstW = totalFirstW * angleWidthFirstBall / resolutionWidthFirstCam;

            //qDebug() << (sizeFirstH);
            //qDebug() << (sizeFirstW);

            /*double angleWidthSecondBall = dataSecondCam[9], angleHeightSecondBall = dataSecondCam[10];
            double distanceToSecondBall =  sqrt((resultDec[0] - secondCamX) * (resultDec[0] - secondCamX) + (resultDec[1] - secondCamY)*(resultDec[1]-secondCamY) + (resultDec[2] - secondCamZ) * (resultDec[2] - secondCamZ) );
            double disSecondCenter = distanceToSecondBall * cos( degToRad(- bSecondCam * positionYSecondCam + fowHeightSecondCam / 2)) * cos(degToRad(aSecondCam * positionXSecondCam - fowWidthSecondCam / 2));

            double totalSecondH = 2 * disSecondCenter * tan(degToRad(fowHeightSecondCam/2));
            double sizeSecondH = totalSecondH * angleHeightSecondBall / resolutionHeightSecondCam;

            double totalSecondW = 2 * disSecondCenter * tan(degToRad(fowWidthSecondCam/2));
            double sizeSecondW = totalSecondW * angleWidthSecondBall / resolutionWidthSecondCam;*/
            //qDebug() << (sizeSecondH);
            //qDebug() << (sizeSecondW);

            QVector <double> totalResult = {resultGeoCoord.latitude(), resultGeoCoord.longitude(), resultDec[2] + dataFirstCam[2], sizeFirstW, sizeFirstH};

            return totalResult;
        }
        else
        {
            return QVector<double>{};
        }
    }

    bool equalVector(QVector <double> firstVec, QVector <double> secondVec, double eps)
    {
        double summ = 0;
        if (firstVec.size() == secondVec.size())
        {
            for (int i = 0; i < firstVec.size(); ++i)
            {
                summ += (firstVec[i] - secondVec[i]) * (firstVec[i] - secondVec[i]);
            }
            return summ < eps;
        }
        else return 0;

    }

    double det (QVector<QVector <double>> mtrx) {
        return mtrx[vecX][vecX]*mtrx[vecY][vecY]*mtrx[vecZ][vecZ] + mtrx[vecX][vecY]*mtrx[vecY][vecZ]*mtrx[vecZ][vecX]
            + mtrx[vecY][vecX]*mtrx[vecZ][vecY]*mtrx[vecX][vecZ] - mtrx[vecZ][vecX]*mtrx[vecY][vecY]*mtrx[vecX][vecZ] -
            mtrx[vecZ][vecY]*mtrx[1][vecZ]*mtrx[vecX][vecX] - mtrx[vecY][vecX]*mtrx[vecX][vecY]*mtrx[vecZ][vecZ];
    }

    QVector <double> kramer(QVector<QVector <double>> matrix, QVector <double> vec)
    {

        QVector <QVector <double>> matrixFirst = {QVector <double> {vec[vecX], matrix[vecX][1], matrix[vecX][vecZ]},
                                                  QVector <double> {vec[vecY], matrix[vecY][vecY], matrix[vecY][vecZ]},
                                                  QVector <double> {vec[vecZ], matrix[vecZ][vecY], matrix[vecZ][vecZ]}},
                                   matrixSecond = {QVector <double> { matrix[vecX][vecX], vec[vecX], matrix[vecX][vecZ]},
                                                   QVector <double> { matrix[vecY][vecX], vec[vecY], matrix[vecY][vecZ]},
                                                   QVector <double> { matrix[vecZ][vecX], vec[vecZ], matrix[vecZ][vecZ]}},
                                   matrixThird = {QVector <double> { matrix[vecX][vecX], matrix[vecX][vecY], vec[vecX]},
                                                  QVector <double> { matrix[vecY][vecX], matrix[vecY][vecY], vec[vecY]},
                                                  QVector <double> { matrix[vecZ][vecX], matrix[vecZ][vecY], vec[vecZ]}};

        double detMain = det(matrix), detFirst = det(matrixFirst), detSecond = det(matrixSecond), detThird = det(matrixThird);

        return QVector <double> {detFirst / detMain, detSecond / detMain, detThird / detMain};
    }

}
