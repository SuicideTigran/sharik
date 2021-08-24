#ifndef SHARIK_H
#define SHARIK_H
#include "mainwindow.h"
#include <QGroupBox>

//void choose_file();

class QPushButton;
class QLabel;

class Sharik : public QGroupBox
{
Q_OBJECT
public:
    Sharik(QWidget* pwgt = 0);

public slots:
    void slotFirstButtonClicked();
    void slotSecondButtonClicked();
    void slotResultButtonClicked();

private:
    QPushButton* butFirstCam;
    QPushButton* butSecondCam;

    QListWidget *listFirstCam;
    QListWidget *listSecondCam;

    QVector<double> dataFirstCam;
    QVector<double> dataSecondCam;

    QPushButton* butResult;
    QVector<double> dataResultCam;
    QListWidget *listResult;

private:
    QVector<double> dataFromFile();
};

#endif // SHARIK_H
