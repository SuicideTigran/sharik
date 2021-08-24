#include "sharikdialog.h"
#include "ui_sharikdialog.h"

#include <QVector>




sharikDialog::sharikDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sharikDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &sharikDialog::slotFirstButtonClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &sharikDialog::slotSecondButtonClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &sharikDialog::slotResultButtonClicked);
}

sharikDialog::~sharikDialog()
{
    delete ui;
}

QVector<double> sharikDialog::dataFromFile(){
    // Файловое диалоговое окно
    QString str = QFileDialog::getOpenFileName(0, "", "", "*.csv");

    if(!str.isEmpty())
    {
        // Считывание из csv файла
        QFile file1(str);

        if (!file1.open(QIODevice::ReadOnly))
        {
            qDebug() << "Ошибка чтения файла!";
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

void sharikDialog::slotFirstButtonClicked()
{
    QVector<QString> words = {"Широта видеокамеры:", "Долгота видеокамеры:", "Высота видеокамеры:", "Азимут:", "Угол места:", "Число пикселей по горизонтали:", "Число пикселей по вертикали:", "Расположение шарика по оси абцис:", "Расположение шарика по оси ординат:", "Размер по горизонатли:", "Размер по вертикали:", "Ширина поля зрения:", "Высота поля зрения:"};
    dataFirstCam = dataFromFile();
    if(!dataFirstCam.isEmpty())
    {
        ui->listWidget->clear();
        for(int i = 0; i < words.size(); i++)
        {
            ui->listWidget->addItem(words[i] + "   " + QString::number(dataFirstCam[i]));
        }
    }
}

void sharikDialog::slotSecondButtonClicked()
{
    QVector<QString> words = {"Широта видеокамеры:", "Долгота видеокамеры:", "Высота видеокамеры:", "Азимут:", "Угол места:", "Число пикселей по горизонтали:", "Число пикселей по вертикали:", "Расположение шарика по оси абцис:", "Расположение шарика по оси ординат:", "Размер по горизонатли:", "Размер по вертикали:", "Ширина поля зрения:", "Высота поля зрения:"};
    dataSecondCam = dataFromFile();
    if(!dataSecondCam.isEmpty())
    {
        ui->listWidget_4->clear();
        for(int i = 0; i < words.size(); i++)
        {
            ui->listWidget_4->addItem(words[i] + "   " + QString::number(dataSecondCam[i]));
        }
    }
}

void sharikDialog::slotResultButtonClicked()
{
    QVector<QString> resWords = {"Широта:", "Долгота:", "Высота:", "Ширина:", "Высота:"};
    dataResultCam.clear();
    dataResultCam.append(0);dataResultCam.append(0);dataResultCam.append(0);dataResultCam.append(0);dataResultCam.append(0);

    if(dataFirstCam.isEmpty() || dataSecondCam.isEmpty())
    {
        ui->listWidget_3->setStyleSheet("QListWidget {"
                                     "border-style: solid;"
                                     "border-width: 1px;"
                                     "border-color: black; "
                                     "background-color: red"
                                     "}");
        ui->listWidget_3->clear();
        ui->listWidget_3->addItem("\n\nЗагрузите данные всех камер!\n\n");
        //lblResult->setAlignment(Qt::AlignHCenter);
        //ui->listWidget_3->currentItem()->setTextAlignment(Qt::AlignCenter);
    }
    else
    {
        ui->listWidget_3->setStyleSheet("QListWidget {"
                                     "border-style: solid;"
                                     "border-width: 1px;"
                                     "border-color: black; "
                                     "background-color: white"
                                     "}");
        //lblResult->setAlignment(Qt::AlignLeft);
        ui->listWidget_3->clear();

        for(int i = 0; i < resWords.size(); i++)
        {
            ui->listWidget_3->addItem(resWords[i] + "   " + QString::number(dataResultCam[i]));
        }
    }
}

