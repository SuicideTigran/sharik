#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QFile>
#include <QFileDialog>
#include <QVector>
#include <QGeoCoordinate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

    void slotFirstButtonClicked();

    void slotSecondButtonClicked();

    void on_actionProgramm_triggered();

    void on_actionQt_triggered();

    void on_actionClear_triggered();

private:
    QVector<double> dataFirstCam;
    QVector<double> dataSecondCam;
    QVector<double> dataResultCam;

private:
    void resultClick();
    QVector<double> dataFromFile();
    QVector<double> calculateResult();
    bool checkGauss(QVector<QVector <double>> matrix, QVector <double> vec);
    QVector<double> gauss(QVector<QVector <double>> a, QVector<double> y, int n);
    double degToRad(double D);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
