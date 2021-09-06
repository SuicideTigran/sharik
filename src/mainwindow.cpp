#include "mainwindow.h"

#include <QMessageBox>
#include <QMap>
#include <QFile>
#include <QFileDialog>
#include <QGeoCoordinate>

#include "ui_mainwindow.h"
#include "mathematics.h"

namespace {
    const double numInputData = 13;

    QVector<QString> strCam = {"Широта видеокамеры:", "Долгота видеокамеры:", "Высота видеокамеры:", "Азимут:",
                               "Угол места:", "Число пикселей по горизонтали:", "Число пикселей по вертикали:",
                               "Расположение шарика по оси абцис:", "Расположение шарика по оси ординат:",
                               "Размер по горизонтали:", "Размер по вертикали:", "Ширина поля зрения:", "Высота поля зрения:"};
    QVector<QString> strResult = {"Широта:", "Долгота:", "Высота:", "Ширина:", "Высота:"};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //sharik = new Sharik;
    this->setWindowTitle("Balloon");
    connect(ui->firstCamButton, &QPushButton::clicked, this, &MainWindow::slotFirstButtonClicked);
    connect(ui->secondCamButton, &QPushButton::clicked, this, &MainWindow::slotSecondButtonClicked);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::slotCloseButtonClicked);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::slotClearButtonClicked);
    connect(ui->actionQt, &QAction::triggered, this, &MainWindow::slotAboutQtButtonClicked);
    connect(ui->actionProgramm, &QAction::triggered, this, &MainWindow::slotAboutProgramButtonClicked);
    connect(ui->resultButton, &QPushButton::clicked, this, &MainWindow::slotResultButtonClicked);

    //QHBoxLayout *layout = new QHBoxLayout(this);
    //layout->addWidget(ui->firstCamButton);
    //layout->addWidget(ui->secondCamButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotCloseButtonClicked()
{
    close();
}


void MainWindow::slotResultButtonClicked()
{
    dataFirstCam.clear(); dataSecondCam.clear();
    dataFirstCam.push_back(ui->latFirstCam->value()); dataSecondCam.push_back(ui->latSecondCam->value());
    dataFirstCam.push_back(ui->lonFirstCam->value()); dataSecondCam.push_back(ui->lonSecondCam->value());
    dataFirstCam.push_back(ui->hFirstCam->value()); dataSecondCam.push_back(ui->hSecondCam->value());
    dataFirstCam.push_back(ui->azimuthFirstCam->value()); dataSecondCam.push_back(ui->azimuthSecondCam->value());
    dataFirstCam.push_back(ui->evelationFirstCam->value()); dataSecondCam.push_back(ui->evelationSecondCam->value());
    dataFirstCam.push_back(ui->resolutionWidthFirstCam->value()); dataSecondCam.push_back(ui->resolutionWidthSecondCam->value());
    dataFirstCam.push_back(ui->resolutionHeightFirstCam->value()); dataSecondCam.push_back(ui->resolutionHeightSecondCam->value());
    dataFirstCam.push_back(ui->posistionXFirstCam->value()); dataSecondCam.push_back(ui->posistionXSecondCam->value());
    dataFirstCam.push_back(ui->posistionYFirstCam->value()); dataSecondCam.push_back(ui->posistionYSecondCam->value());
    dataFirstCam.push_back(ui->sizeWidthFirstCam->value()); dataSecondCam.push_back(ui->sizeWidthSecondCam->value());
    dataFirstCam.push_back(ui->sizeHeightFirstCam->value()); dataSecondCam.push_back(ui->sizeHeightSecondCam->value());
    dataFirstCam.push_back(ui->fowWidthFirstCam->value()); dataSecondCam.push_back(ui->fowWidthSecondCam->value());
    dataFirstCam.push_back(ui->fowHeightSecondCam->value()); dataSecondCam.push_back(ui->fowHeightSecondCam->value());

    resultClick();
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
    using namespace ::shmath;

    dataFirstCam = dataFromFile();
    if(dataFirstCam.size() == numInputData)
    {
        ui->latFirstCam->setValue(dataFirstCam[lat]);
        ui->lonFirstCam->setValue(dataFirstCam[lon]);
        ui->hFirstCam->setValue(dataFirstCam[hgt]);
        ui->azimuthFirstCam->setValue(dataFirstCam[azim]);
        ui->evelationFirstCam->setValue(dataFirstCam[elevation]);
        ui->resolutionWidthFirstCam->setValue(dataFirstCam[resolutionW]);
        ui->resolutionHeightFirstCam->setValue(dataFirstCam[resolutionH]);
        ui->posistionXFirstCam->setValue(dataFirstCam[positionX]);
        ui->posistionYFirstCam->setValue(dataFirstCam[positionY]);
        ui->sizeWidthFirstCam->setValue(dataFirstCam[sizeW]);
        ui->sizeHeightFirstCam->setValue(dataFirstCam[sizeH]);
        ui->fowWidthFirstCam->setValue(dataFirstCam[fowW]);
        ui->fowHeightFirstCam->setValue(dataFirstCam[fowH]);

        if(!dataSecondCam.isEmpty()) resultClick();
    }
    else if (!dataFirstCam.isEmpty())
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("Некорректные данные!");
        msg->exec();
    }

}

void MainWindow::slotSecondButtonClicked()
{
    using namespace ::shmath;

    dataSecondCam = dataFromFile();
    if(dataSecondCam.size() == numInputData)
    {
        ui->latSecondCam->setValue(dataSecondCam[lat]);
        ui->lonSecondCam->setValue(dataSecondCam[lon]);
        ui->hSecondCam->setValue(dataSecondCam[hgt]);
        ui->azimuthSecondCam->setValue(dataSecondCam[azim]);
        ui->evelationSecondCam->setValue(dataSecondCam[elevation]);
        ui->resolutionWidthSecondCam->setValue(dataSecondCam[resolutionW]);
        ui->resolutionHeightSecondCam->setValue(dataSecondCam[resolutionH]);
        ui->posistionXSecondCam->setValue(dataSecondCam[positionX]);
        ui->posistionYSecondCam->setValue(dataSecondCam[positionY]);
        ui->sizeWidthSecondCam->setValue(dataSecondCam[sizeW]);
        ui->sizeHeightSecondCam->setValue(dataSecondCam[sizeH]);
        ui->fowWidthSecondCam->setValue(dataSecondCam[fowW]);
        ui->fowHeightSecondCam->setValue(dataSecondCam[fowH]);

        if(!dataFirstCam.isEmpty()) resultClick();
    }
    else if (!dataSecondCam.isEmpty())
    {
        QMessageBox msg;
        msg.setText("Некорректные данные!");
        msg.exec();
    }
}

void MainWindow::resultClick()
{
    using namespace ::shmath;

    dataResultCam.clear();
    if(equalVector(dataFirstCam, dataSecondCam, 0.00000001))
    {
        QMessageBox msg;
        msg.setText("Загружены данные одинаковых камер!");
        msg.exec();

        ui->resultList->clear();
        for(int i = 0; i < strResult.size(); i++)
        {
            ui->resultList->addItem(strResult[i] + "  -");
        }
    }
    else
    {
        dataResultCam = calculateResult(dataFirstCam, dataSecondCam);
        //qDebug() << "==============================";
        if (dataResultCam.isEmpty())
        {
            //qDebug() << "!!!!!!!!!!!!!!!!";
            QMessageBox *msg = new QMessageBox;
            msg->setText("Некорректные данные1!");
            msg->exec();
            ui->resultList->clear();
            for(int i = 0; i < strResult.size(); i++)
            {
                ui->resultList->addItem(strResult[i] + "  -");
            }
        }
        else
        {
            //qDebug() << "??????????????????????";
            ui->resultList->clear();

            for(int i = 0; i < strResult.size(); i++)
            {
                ui->resultList->addItem(strResult[i] + "   " + QString::number(dataResultCam[i]));
            }
        }
    }


}

void MainWindow::slotAboutProgramButtonClicked()
{
    QMessageBox::information(this, "О программе", "Версия 1.0\nДанная программа разработана в Qt 5.12.2");
}

void MainWindow::slotClearButtonClicked()
{
    dataFirstCam.clear();
    ui->latFirstCam->setValue(0);
    ui->lonFirstCam->setValue(0);
    ui->hFirstCam->setValue(0);
    ui->azimuthFirstCam->setValue(0);
    ui->evelationFirstCam->setValue(0);
    ui->resolutionWidthFirstCam->setValue(0);
    ui->resolutionHeightFirstCam->setValue(0);
    ui->posistionXFirstCam->setValue(0);
    ui->posistionYFirstCam->setValue(0);
    ui->sizeWidthFirstCam->setValue(0);
    ui->sizeHeightFirstCam->setValue(0);
    ui->fowWidthFirstCam->setValue(0);
    ui->fowHeightFirstCam->setValue(0);

    dataSecondCam.clear();
    ui->latSecondCam->setValue(0);
    ui->lonSecondCam->setValue(0);
    ui->hSecondCam->setValue(0);
    ui->azimuthSecondCam->setValue(0);
    ui->evelationSecondCam->setValue(0);
    ui->resolutionWidthSecondCam->setValue(0);
    ui->resolutionHeightSecondCam->setValue(0);
    ui->posistionXSecondCam->setValue(0);
    ui->posistionYSecondCam->setValue(0);
    ui->sizeWidthSecondCam->setValue(0);
    ui->sizeHeightSecondCam->setValue(0);
    ui->fowWidthSecondCam->setValue(0);
    ui->fowHeightSecondCam->setValue(0);

    dataResultCam.clear();
    ui->resultList->clear();
    for(int i = 0; i < strResult.size(); i++)
    {
        ui->resultList->addItem(strResult[i] + "  -");
    }
}

void MainWindow::slotAboutQtButtonClicked()
{
    QApplication::aboutQt();
}


