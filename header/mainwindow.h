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

    QVector<double> dataFirstCam;
    QVector<double> dataSecondCam;
    QVector<double> dataResultCam;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
