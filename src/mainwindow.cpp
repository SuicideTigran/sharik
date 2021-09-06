#include "mainwindow.h"

#include <QMessageBox>
#include <QMap>
#include <QFile>
#include <QFileDialog>
#include <QGeoCoordinate>

#include "ui_mainwindow.h"
#include "mathematics.h"

namespace {
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

    fillTaggedSpinboxes();

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
    dataFirstCam.clear();
    dataSecondCam.clear();

    const auto sb_value_f = [] (QDoubleSpinBox * sb) { return sb->value(); };

    std::transform (std::begin (firstCamSpinBoxes), std::end (firstCamSpinBoxes),
                    std::back_inserter<decltype(dataFirstCam)> (dataFirstCam),
                    sb_value_f);

    std::transform (std::begin (secondCamSpinBoxes), std::end (secondCamSpinBoxes),
                    std::back_inserter<decltype(dataSecondCam)> (dataSecondCam),
                    sb_value_f);

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

void MainWindow::fillTaggedSpinboxes()
{
    using namespace ::shmath;

    const indexDataCam indexes[] = {
        lat, lon, hgt,
        azim, elevation,
        resolutionW, resolutionH,
        positionX, positionY,
        sizeW, sizeH,
        fowW, fowH
    };

    QDoubleSpinBox* firstCamSpinBoxesArray[] = {
        ui->latFirstCam, ui->lonFirstCam, ui->hFirstCam,
        ui->azimuthFirstCam, ui->evelationFirstCam,
        ui->resolutionWidthFirstCam, ui->resolutionHeightFirstCam,
        ui->posistionXFirstCam, ui->posistionYFirstCam,
        ui->sizeWidthFirstCam, ui->sizeHeightFirstCam,
        ui->fowWidthFirstCam, ui->fowHeightFirstCam
    };

    QDoubleSpinBox* secondCamSpinBoxesArray[] = {
        ui->latSecondCam, ui->lonSecondCam, ui->hSecondCam,
        ui->azimuthSecondCam, ui->evelationSecondCam,
        ui->resolutionWidthSecondCam, ui->resolutionHeightSecondCam,
        ui->posistionXSecondCam, ui->posistionYSecondCam,
        ui->sizeWidthSecondCam, ui->sizeHeightSecondCam,
        ui->fowWidthSecondCam, ui->fowHeightSecondCam
    };

    for (const auto index : indexes)
    {
        firstCamSpinBoxes[index] = firstCamSpinBoxesArray[index];
        secondCamSpinBoxes[index] = secondCamSpinBoxesArray[index];
    }
}

void loadCamSlotHelper (QVector<double>* data, std::array<QDoubleSpinBox*, shmath::indexDataCamLength>* sboxes)
{
    using namespace ::shmath;

    if(data->size() == static_cast<int> (sboxes->size()))
    {
        for (auto index = 0; index < data->size(); ++index)
        {
            (*sboxes)[index]->setValue((*data)[index]);
        }
    }
    else if (!data->isEmpty())
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("Некорректные данные!");
        msg->exec();
    }
}

void MainWindow::slotFirstButtonClicked()
{
    dataFirstCam = dataFromFile();

    loadCamSlotHelper (&dataFirstCam, &firstCamSpinBoxes);

    if (! (dataFirstCam.isEmpty() || dataSecondCam.isEmpty()))
    {
        resultClick();
    }
}

void MainWindow::slotSecondButtonClicked()
{
    dataSecondCam = dataFromFile();

    loadCamSlotHelper (&dataSecondCam, &secondCamSpinBoxes);

    if (! (dataFirstCam.isEmpty() || dataSecondCam.isEmpty()))
    {
        resultClick();
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
    for (auto sb: firstCamSpinBoxes) { sb->setValue(0); }

    dataSecondCam.clear();
    for (auto sb : secondCamSpinBoxes) { sb->setValue(0); }

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


