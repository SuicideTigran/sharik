#include "mainwindow.h"
#include "sharik.h"
#include "mainwindow.h"

#include <QApplication>
#include <QWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //sharikDialog *b = new sharikDialog();
    //b->show();

    MainWindow *mWin = new MainWindow();
    mWin->show();

    //Sharik sharik;
    //sharik.show();

    return a.exec();
}
