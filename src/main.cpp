#include "mainwindow.h"

#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mWin;
    mWin.show();

    return a.exec();
}
