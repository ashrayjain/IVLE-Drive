#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ivleconnector.h"
#include "storage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    IVLEConnector* ivle = new IVLEConnector(Storage::readToken(), this);

}

MainWindow::~MainWindow()
{
    delete ui;
}
