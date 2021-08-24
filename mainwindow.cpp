#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //sharik = new Sharik;
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::slotButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotButtonClicked()
{

    sharikDialog *b = new sharikDialog;
    b->show();

}


void MainWindow::on_actionQuit_triggered()
{
    close();
}


void MainWindow::on_actionNew_Sharik_triggered()
{
    sharikDialog *b = new sharikDialog;
    b->show();
}

