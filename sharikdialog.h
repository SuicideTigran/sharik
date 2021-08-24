#ifndef SHARIKDIALOG_H
#define SHARIKDIALOG_H
#include "mainwindow.h"
#include <QDialog>



namespace Ui {
class sharikDialog;
}

class sharikDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sharikDialog(QWidget *parent = nullptr);
    ~sharikDialog();
public slots:
    void slotFirstButtonClicked();
    void slotSecondButtonClicked();
    void slotResultButtonClicked();

private:
    QVector<double> dataFirstCam;
    QVector<double> dataSecondCam;
    QVector<double> dataResultCam;

private:
    QVector<double> dataFromFile();

private:
    Ui::sharikDialog *ui;
};

#endif // SHARIKDIALOG_H
