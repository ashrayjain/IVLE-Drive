#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ivleconnector.h"
#include "ivlelogindialog.h"
#include "storage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ivle = new IVLEConnector(Storage::readToken());
    on_syncButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ivle;
}

void MainWindow::slotTokenChange(QString newToken) {
    dynamic_cast<QDialog*>(sender())->close();
    delete ivle;
    ivle = new IVLEConnector(newToken);
    Storage::writeToken(newToken);
    ivle->getWorkbins();
}

void MainWindow::on_actionLogin_triggered()
{
    IVLELoginDialog* loginDlg = new IVLELoginDialog(this);
    connect(loginDlg, SIGNAL(tokenChanged(QString)), this, SLOT(slotTokenChange(QString)));
    loginDlg->exec();
    loginDlg->close();
}

void MainWindow::on_syncButton_clicked()
{
    if(!ivle->getWorkbins()) {
        on_actionLogin_triggered();
        ivle->getWorkbins();
    }
}
