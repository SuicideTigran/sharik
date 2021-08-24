#include "sharik.h"
#include <QVector>

QVector<QString> words = {"Широта видеокамеры:", "Долгота видеокамеры:", "Высота видеокамеры:", "Азимут:", "Угол места:", "Число пикселей по горизонтали:", "Число пикселей по вертикали:", "Расположение шарика по оси абцис:", "Расположение шарика по оси ординат:", "Размер по горизонатли:", "Размер по вертикали:", "Ширина поля зрения:", "Высота поля зрения:"};
QVector<QString> resWords = {"Широта:", "Долгота:", "Высота:", "Ширина:", "Высота:"};

QVector<double> Sharik::dataFromFile(){
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

Sharik::Sharik(QWidget* pwgt) : QGroupBox("", pwgt)
{
    //Кнопки камер
    butFirstCam = new QPushButton("&Загрузить первую камеру");
    butSecondCam = new QPushButton("&Загрузить вторую камеру");
    connect(butFirstCam, &QPushButton::clicked, this, &Sharik::slotFirstButtonClicked);
    connect(butSecondCam, &QPushButton::clicked, this, &Sharik::slotSecondButtonClicked);

     listFirstCam = new QListWidget;
     listSecondCam = new QListWidget;
     listFirstCam->setMinimumSize(350, 310);
     listSecondCam->setMinimumSize(350, 310);
     listFirstCam->setStyleSheet("QListWidget {"
                                  "border-style: solid;"
                                  "border-width: 1px;"
                                  "border-color: black; "
                                  "}");
     listSecondCam->setStyleSheet("QListWidget {"
                                  "border-style: solid;"
                                  "border-width: 1px;"
                                  "border-color: black; "
                                  "}");
     for(int i = 0; i < 13; i++)
     {
         listFirstCam->addItem(words[i] + "   -");
         listSecondCam->addItem(words[i] + "   -");
     }

    //Надписи камер
    QLabel *headFirstCam = new QLabel("Параметры первой камеры");
    QLabel *headSecondCam = new QLabel("Параметры второй камеры");

    //Надпись результата
    QLabel *headResult = new QLabel("\nРезультаты");
    //headResult->setAlignment(Qt::AlignHCenter);
    listResult = new QListWidget;
    listResult->setStyleSheet("QListWidget {"
                                 "border-style: solid;"
                                 "border-width: 1px;"
                                 "border-color: black; "
                                 "}");
    for(int i = 0; i < 5; i++)
    {
        listResult->addItem(resWords[i] + "   -");
    }


    //Кнопка результата
    butResult = new QPushButton("Расчитать результат");

    connect(butResult, &QPushButton::clicked, this, &Sharik::slotResultButtonClicked);

    //Размеры шрифтов
    QFont font;
    font.setPixelSize(16);
    butFirstCam->setFont(font);
    butSecondCam->setFont(font);
    butResult->setFont(font);
    listFirstCam->setFont(font);
    listSecondCam->setFont(font);
    listResult->setFont(font);
    font.setPixelSize(18);
    headFirstCam->setFont(font);
    headSecondCam->setFont(font);
    headResult->setFont(font);

    //Layout
    QVBoxLayout* pvbLayoutFrist = new QVBoxLayout;
    pvbLayoutFrist->addWidget(headFirstCam);
    pvbLayoutFrist->addWidget(listFirstCam);
    pvbLayoutFrist->addWidget(butFirstCam);

    QVBoxLayout* pvbLayoutSecond = new QVBoxLayout;
    pvbLayoutSecond->addWidget(headSecondCam);
    pvbLayoutSecond->addWidget(listSecondCam);
    pvbLayoutSecond->addWidget(butSecondCam);

    QHBoxLayout* pvbLayout = new QHBoxLayout;
    pvbLayout->addLayout(pvbLayoutFrist);
    pvbLayout->addLayout(pvbLayoutSecond);

    QVBoxLayout* pvbMainLayout = new QVBoxLayout;
    pvbMainLayout->addLayout(pvbLayout);
    pvbMainLayout->addWidget(headResult);
    pvbMainLayout->addWidget(listResult);
    pvbMainLayout->addWidget(butResult);
    setLayout(pvbMainLayout);
}

void Sharik::slotFirstButtonClicked()
{
    dataFirstCam = dataFromFile();
    if(!dataFirstCam.isEmpty())
    {
        listFirstCam->clear();
        for(int i = 0; i < words.size(); i++)
        {
            listFirstCam->addItem(words[i] + "   " + QString::number(dataFirstCam[i]));
        }
    }
}

void Sharik::slotSecondButtonClicked()
{
    dataSecondCam = dataFromFile();
    if(!dataSecondCam.isEmpty())
    {
        listSecondCam->clear();
        for(int i = 0; i < words.size(); i++)
        {
            listSecondCam->addItem(words[i] + "   " + QString::number(dataSecondCam[i]));
        }
    }
}

void Sharik::slotResultButtonClicked()
{
    dataResultCam.clear();
    dataResultCam.append(0);dataResultCam.append(0);dataResultCam.append(0);dataResultCam.append(0);dataResultCam.append(0);

    if(dataFirstCam.isEmpty() || dataSecondCam.isEmpty())
    {
        listResult->setStyleSheet("QListWidget {"
                                     "border-style: solid;"
                                     "border-width: 1px;"
                                     "border-color: black; "
                                     "background-color: red"
                                     "}");
        listResult->clear();
        listResult->addItem("\n\nЗагрузите данные всех камер!\n\n");
        //lblResult->setAlignment(Qt::AlignHCenter);
    }
    else
    {
        listResult->setStyleSheet("QListWidget {"
                                     "border-style: solid;"
                                     "border-width: 1px;"
                                     "border-color: black; "
                                     "background-color: white"
                                     "}");
        //lblResult->setAlignment(Qt::AlignLeft);
        listResult->clear();

        for(int i = 0; i < resWords.size(); i++)
        {
            listResult->addItem(resWords[i] + "   " + QString::number(dataResultCam[i]));
        }
    }
}
