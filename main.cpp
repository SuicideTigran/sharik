#include "mainwindow.h"
#include <QApplication>
#include <QWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow *mWin = new MainWindow();
    mWin->show();

    return a.exec();
}
