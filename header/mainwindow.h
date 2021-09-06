#pragma once

#include <QMainWindow>
#include <QVector>

#include "mathematics.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QDoubleSpinBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotFirstButtonClicked();
    void slotSecondButtonClicked();
    void slotCloseButtonClicked();
    void slotClearButtonClicked();
    void slotAboutQtButtonClicked();
    void slotAboutProgramButtonClicked();
    void slotResultButtonClicked();

private:
    void resultClick();
    QVector<double> dataFromFile();
    void fillTaggedSpinboxes ();

    QVector<double> dataFirstCam;
    QVector<double> dataSecondCam;
    QVector<double> dataResultCam;

    std::array<QDoubleSpinBox*, shmath::indexDataCamLength> firstCamSpinBoxes;
    std::array<QDoubleSpinBox*, shmath::indexDataCamLength> secondCamSpinBoxes;

    Ui::MainWindow *ui;
};
