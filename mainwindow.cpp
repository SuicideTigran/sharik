#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

const double numInputData = 13;
QVector<QString> strCam = {"Широта видеокамеры:", "Долгота видеокамеры:", "Высота видеокамеры:", "Азимут:", "Угол места:", "Число пикселей по горизонтали:", "Число пикселей по вертикали:", "Расположение шарика по оси абцис:", "Расположение шарика по оси ординат:", "Размер по горизонтали:", "Размер по вертикали:", "Ширина поля зрения:", "Высота поля зрения:"};
QVector<QString> strResult = {"Широта:", "Долгота:", "Высота:", "Ширина:", "Высота:"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //sharik = new Sharik;

    connect(ui->firstCamButton, &QPushButton::clicked, this, &MainWindow::slotFirstButtonClicked);
    connect(ui->secondCamButton, &QPushButton::clicked, this, &MainWindow::slotSecondButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionQuit_triggered()
{
    close();
}



QVector<double> MainWindow::dataFromFile(){
    // Файловое диалоговое окно
    QString str = QFileDialog::getOpenFileName(0, "", "", "*.csv");

    if(!str.isEmpty())
    {
        // Считывание из csv файла
        QFile file1(str);

        if (!file1.open(QIODevice::ReadOnly))
        {
            QMessageBox *msg = new QMessageBox;
            msg->setText("Ошибка чтения файла!");
            msg->exec();
        }

        QRegExp rx("[^/.\\d]");
        QString line;
        QVector<double> doubleData;
        while(!file1.atEnd())
        {
            line = file1.readLine();
        }
        for (QString item : line.split(";"))
        {
            item.replace(rx, "");
            if(item.isEmpty()) return QVector<double>{};
            doubleData.append(item.toDouble()); // Вектор значений из csv файлов
        }
        file1.close();
        return doubleData;
    }
    else
    {
        return QVector<double>{};
    }
}

void MainWindow::slotFirstButtonClicked()
{
    dataFirstCam = dataFromFile();
    if(dataFirstCam.size() == numInputData)
    {
        ui->firstCamList->clear();
        for(int i = 0; i < strCam.size(); i++)
        {
            ui->firstCamList->addItem(strCam[i] + "   " + QString::number(dataFirstCam[i]));
        }
        if(!dataSecondCam.isEmpty()) resultClick();
    }
    else
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("Некорректные данные!");
        msg->exec();
    }
}

void MainWindow::slotSecondButtonClicked()
{
    dataSecondCam = dataFromFile();
    if(dataSecondCam.size() == numInputData)
    {
        ui->secondCamList->clear();
        for(int i = 0; i < strCam.size(); i++)
        {
            ui->secondCamList->addItem(strCam[i] + "   " + QString::number(dataSecondCam[i]));
        }
        if(!dataFirstCam.isEmpty()) resultClick();
    }
    else
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("Некорректные данные!");
        msg->exec();
    }
}

void MainWindow::resultClick()
{
   dataResultCam.clear();

    if(dataFirstCam == dataSecondCam)
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("Загружены данные одинаковых камер!");
        msg->exec();
        ui->resultList->clear();
        for(int i = 0; i < strResult.size(); i++)
        {
            ui->resultList->addItem(strResult[i] + "  -");
        }
    }
    else
    {
        dataResultCam = calculateResult();
        if (dataResultCam.isEmpty())
        {
            QMessageBox *msg = new QMessageBox;
            msg->setText("Некорректные данные!");
            msg->exec();
            ui->resultList->clear();
            for(int i = 0; i < strResult.size(); i++)
            {
                ui->resultList->addItem(strResult[i] + "  -");
            }
        }
        else
        {
            ui->resultList->clear();

            for(int i = 0; i < strResult.size(); i++)
            {
                ui->resultList->addItem(strResult[i] + "   " + QString::number(dataResultCam[i]));
            }
        }
    }


}

QVector <double> MainWindow::calculateResult()
{
    QGeoCoordinate firstCam(dataFirstCam[0], dataFirstCam[1]), secondCam(dataSecondCam[0], dataSecondCam[1]);
    double distance = firstCam.distanceTo(secondCam);
    double azimuth = firstCam.azimuthTo(secondCam);

    //Перевод в декартову систему координат
    double firstCamX = 0, firstCamY = 0, firstCamZ = 0;
    double secondCamX = distance * sin(degToRad(azimuth));
    double secondCamY = distance * cos(degToRad(azimuth));
    double secondCamZ = dataSecondCam[2] - dataFirstCam[2];

    //qDebug() << "Frist Camera:" << firstCamX << "  " << firstCamY << "  " << firstCamZ;
    //qDebug() << "Second Camera:" << secondCamX << "  " << secondCamY << "  " << secondCamZ;

    double azimuthFirstCam = dataFirstCam[3], elevationFirstCam = dataFirstCam[4], azimuthSecondCam = dataSecondCam[3], elevationSecondCam = dataSecondCam[4];
    double fowWidthFirstCam = dataFirstCam[11], fowHeightFirstCam = dataFirstCam[12], fowWidthSecondCam = dataSecondCam[11], fowHeightSecondCam = dataSecondCam[12];
    double resolutionWidthFirstCam = dataFirstCam[5], resolutionHeightFirstCam = dataFirstCam[6], resolutionWidthSecondCam = dataSecondCam[5], resolutionHeightSecondCam = dataSecondCam[6];
    double positionXFirstCam = dataFirstCam[7], positionYFirstCam = dataFirstCam[8], positionXSecondCam = dataSecondCam[7], positionYSecondCam = dataSecondCam[8];

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
    matrix.push_back(QVector <double> {vecFirstCam[1], -vecFirstCam[0], 0});
    matrix.push_back(QVector <double> {0, -vecFirstCam[2], vecFirstCam[1]});
    matrix.push_back(QVector <double> {vecSecondCam[1], -vecSecondCam[0], 0});
    matrix.push_back(QVector <double> {0, -vecSecondCam[2], vecSecondCam[1]});

    QVector <double> rVec = {vecFirstCam[1] * firstCamX - vecFirstCam[0] * firstCamY,
    vecFirstCam[1] * firstCamZ - vecFirstCam[2] * firstCamY,
    vecSecondCam[1] * secondCamX - vecSecondCam[0] * secondCamY,
    vecSecondCam[1] * secondCamZ - vecSecondCam[2] * secondCamY};

    if(checkGauss(matrix, rVec))
    {
        QVector <double> resultDec = gauss(matrix, rVec, 3);

        //Перевод в географические координаты
        double distanceRes = sqrt(resultDec[0]*resultDec[0] + resultDec[1]*resultDec[1]);
        QGeoCoordinate resultGeoCoord = firstCam.atDistanceAndAzimuth(distanceRes, alphaFirstCam);
        //qDebug() << "GeoResult:  " << resultGeoCoord.latitude() << "  " << resultGeoCoord.longitude() << "  " << resultDec[2] + dataFirstCam[2];

        //Поиск высоты и ширины объекта
        double angleWidthFirstBall = dataFirstCam[9], angleHeightFirstBall = dataFirstCam[10];
        double distanceToFirstBall =  sqrt((resultDec[0]) * (resultDec[0]) + (resultDec[1])*(resultDec[1]) + (resultDec[2]) * (resultDec[2]) );
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

double MainWindow::degToRad(double D)
{
    double M = M_PI / 180;
    return D * M;
};

//Решение СЛУ методом Гаусса
QVector<double> MainWindow::gauss(QVector<QVector <double>> a, QVector<double> y, int n)
{
    double  max;
    QVector<double> x;
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
            return QVector<double>{0};
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

void MainWindow::on_actionProgramm_triggered()
{
    QMessageBox::information(this, "О программе", "Версия 1.0\nДанная программа разработана в Qt 5.12.2");
}


void MainWindow::on_actionQt_triggered()
{
    QApplication::aboutQt();
}


void MainWindow::on_actionClear_triggered()
{
    ui->firstCamList->clear();
    ui->secondCamList->clear();
    dataFirstCam.clear();
    dataSecondCam.clear();
    for(int i = 0; i < strCam.size(); i++)
    {
        ui->firstCamList->addItem(strCam[i] + "  -");
        ui->secondCamList->addItem(strCam[i] + "  -");
    }

    dataResultCam.clear();
    ui->resultList->clear();
    for(int i = 0; i < strResult.size(); i++)
    {
        ui->resultList->addItem(strResult[i] + "  -");
    }
}

//Проверка на существование решения
bool MainWindow::checkGauss(QVector<QVector <double>> matrix, QVector <double> vec)
{
    QVector <QVector <double>> matFirst, matSecond;
    QVector <double> vecFirst, vecSecond;
    for(int i = 0; i < matrix.size() - 1; ++i)
    {
        QVector <double> rowFirst, rowSecond;
        for(int j = 0; j < matrix[i].size(); ++j)
        {
            rowFirst.push_back(matrix[i][j]);
            rowSecond.push_back(matrix[i+1][j]);
        }
        matFirst.push_back(rowFirst);
        matSecond.push_back(rowSecond);
        vecFirst.push_back(vec[i]);
        vecSecond.push_back(vec[i+1]);
    }

    QVector <double> resFirst = gauss(matFirst, vecFirst, 3), resSecond = gauss(matSecond, vecSecond, 3);
    double eps = 0;
    for(int i = 0; i < resFirst.size(); ++i)
    {
        eps += (resFirst[i] - resSecond[i]) * (resFirst[i] - resSecond[i]);
    }
    return (eps < 1);
}
