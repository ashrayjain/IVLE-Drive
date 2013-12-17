#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ivleconnector.h"
#include "ivlelogindialog.h"

// Green - #98FF75

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ivle(new IVLEConnector(this))
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Trying to connect with IVLE..");
    connect(ivle, SIGNAL(tokenChanged(bool)), this, SLOT(slotTokenChanged(bool)));
    ivle->initiateTokenValidation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotTokenChanged(bool tokenValidity) {
    if (tokenValidity)
        ivle->getWorkbins();
    else
        initIVLELoginDlg();
}

void MainWindow::initIVLELoginDlg() {
    IVLELoginDialog* dlg = new IVLELoginDialog(this);
    connect(dlg, SIGNAL(newTokenReceived(QString)), this, SLOT(newTokenReceived(QString)));
    dlg->exec();
    dlg->close();
}

void MainWindow::newTokenReceived(QString newToken) {
    dynamic_cast<QDialog*>(sender())->close();
    ivle->setToken(newToken);
}

void MainWindow::loginRequired()
{
    IVLELoginDialog* loginDlg = new IVLELoginDialog(this);
    connect(loginDlg, SIGNAL(tokenChanged(QString)), this, SLOT(slotTokenChanged(QString)));
    loginDlg->exec();
    loginDlg->close();
}

void MainWindow::on_syncButton_clicked()
{
    if(!ivle->getWorkbins()) {
        loginRequired();
        ivle->getWorkbins();
    }
}
