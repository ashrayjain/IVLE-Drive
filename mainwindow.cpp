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
    ui->syncButton->setDisabled(true);
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
        on_syncButton_clicked();
    else
        initIVLELoginDlg();
}

void MainWindow::on_syncButton_clicked() {
    ui->statusBar->showMessage("Syncing..");
    ivle->syncWorkbins();
    //ui->treeView->header()->model()->setHeaderData(0, Qt::Orientation::Horizontal, QVariant("List of Workbins"));
    //ui->treeView->setModel(model);
}

void MainWindow::syncCompleted() {
    ui->statusBar->showMessage("In Sync");
    ui->syncButton->setDisabled(false);
}

void MainWindow::initIVLELoginDlg() {
    IVLELoginDialog* dlg = new IVLELoginDialog(this);
    connect(dlg, SIGNAL(newTokenReceived(QString)), this, SLOT(newTokenReceived(QString)));
    dlg->exec();
    dlg->close();
}

void MainWindow::newTokenReceived(QString newToken) {
    qobject_cast<QDialog*>(sender())->close();
    ivle->setToken(newToken);
}
