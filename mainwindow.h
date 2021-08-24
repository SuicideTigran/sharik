#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QFile>
#include <QFileDialog>
#include <QVector>
#include "sharikdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slotButtonClicked();

private slots:
    void on_actionQuit_triggered();

    void on_actionNew_Sharik_triggered();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
