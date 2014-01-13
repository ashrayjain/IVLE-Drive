#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ivleconnector.h"
#include "ivlelogindialog.h"
#include "filelistmodel.h"
#include "workbinsviewitem.h"
#include "workbinsviewmodel.h"
#include "downloadsmanager.h"
#include "dirmover.h"
#include "storage.h"

// Green - #98FF75

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->syncButton->setDisabled(true);
    ui->browseButton->setDisabled(true);
    ui->statusBar->showMessage("Trying to connect with IVLE...");
    ui->progressBar->setVisible(false);
    connect(ui->fileListView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(fileOpener(QModelIndex)));

    man = new DownloadsManager(this);
    connect(man, SIGNAL(downloadDirChanged(QString)), ui->currentDir, SLOT(setText(QString)));
    connect(man, SIGNAL(progress(int)), this, SLOT(syncProgressChange(int)));
    man->initManager();

    ivle = new IVLEConnector(this);
    connect(ivle, SIGNAL(tokenChanged(bool)), this, SLOT(slotTokenChanged(bool)));
    ivle->initiateTokenValidation();
}

MainWindow::~MainWindow()
{
    Storage::saveState(getState());
    delete ui;
}

void MainWindow::slotTokenChanged(bool tokenValidity) {
    ui->syncButton->setVisible(true);
    if (tokenValidity)
        loadModules();
    else
        initIVLELoginDlg();
}

void MainWindow::on_syncButton_clicked() {
    ui->statusBar->showMessage("Syncing...");
    ui->syncButton->setVisible(false);
    ui->progressBar->setValue(10);
    ui->progressBar->setVisible(true);
    ui->browseButton->setDisabled(true);
    connect(ui->workbinsView, SIGNAL(clicked(QModelIndex)), this, SLOT(refreshFileView(QModelIndex)));
    syncModules();
}

void MainWindow::loadModules()
{
    ui->statusBar->showMessage("Loading Modules...");
    ui->syncButton->setDisabled(true);
    ui->browseButton->setDisabled(true);
    connect(ivle, SIGNAL(loadCompleted()), this, SLOT(loadCompleted()));
    connect(ui->workbinsView, SIGNAL(clicked(QModelIndex)), this, SLOT(refreshFileView(QModelIndex)));
    ivle->loadModules();
}

void MainWindow::syncModules()
{
    WorkbinsViewModel* model = qobject_cast<WorkbinsViewModel*>(ui->workbinsView->model());
    WorkbinsViewItem* rootItem = static_cast<WorkbinsViewItem*>(model->rootIndex().internalPointer());
    man->changeToDir("IVLEDrive");
    for (int i = 0; i < rootItem->childCount(); i++)
        syncModule(rootItem->child(i));
    man->upDir();
    man->executeDownloads(ivle->token());
}

void MainWindow::syncModule(WorkbinsViewItem * node)
{
    if (node->data(Qt::CheckStateRole) != Qt::Unchecked) {
        man->changeToDir(node->data(0).toString()+" "+node->data(1).toString());
        for (int i = 0; i < node->childCount(); i++)
            syncFolder(node->child(i));
        man->upDir();
    }
    else
        man->removeDir(node->data(0).toString()+" "+node->data(1).toString());
}

void MainWindow::syncFolder(WorkbinsViewItem * node)
{
    if (node->data(Qt::CheckStateRole) != Qt::Unchecked) {
        man->changeToDir(node->data(0).toString());
        for (int i = 0; i < node->childCount(); i++)
            syncFolder(node->child(i));

        FileListModel* files = node->fileViewModel();
        for (int i = 0; i < files->rowCount(); i++)
            syncFile(files->getFile(i));
        man->upDir();
    }
    else
        man->removeDir(node->data(0).toString());
}

void MainWindow::syncFile(QJsonObject &file)
{
    if (file["checked"].toBool() == true)
        man->stackDownload(file["id"].toString(), file["name"].toString());
    else if (file["checked"].toBool() == false)
        man->removeFile(file["name"].toString());
}

QString MainWindow::getFilePath(QModelIndex index)
{
    WorkbinsViewItem* node = static_cast<WorkbinsViewItem*>(ui->workbinsView->currentIndex().internalPointer());
    QString filePath = man->dir();
    QStringList childDirs;

    filePath += "/IVLEDrive/";
    while (node->parent()) {
        if (node->data(0) == node->data(1))
            childDirs << node->data(0).toString();
        else {
            childDirs << node->data(0).toString()+" "+node->data(1).toString();
            break;
        }
        node = node->parent();
    }
    for(int k = 0; k < (childDirs.size()/2); k++) childDirs.swap(k,childDirs.size()-(1+k));
    childDirs << index.data().toJsonValue().toString();
    filePath += childDirs.join("/");
    return filePath;
}

void MainWindow::showErrMsg()
{
    QMessageBox msgBox;
    msgBox.setText("File has not been synced yet!");
    msgBox.setInformativeText("Please sync again with this file selected for download.");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void MainWindow::addChild(WorkbinsViewItem *node, QJsonArray &arr)
{
    QJsonObject obj;
    obj["name"] = node->data(0).toString();
    obj["check"] = node->data(Qt::CheckStateRole).toInt();

    QJsonArray _arr;
    for (int i = 0; i < node->childCount(); i++)
        addChild(node->child(i), _arr);

    obj["children"] = _arr;
    obj["files"] = getFiles(node);
    arr.append(obj);
}

QJsonObject MainWindow::getState()
{
    QJsonObject state;
    WorkbinsViewItem* root = static_cast<WorkbinsViewItem*>(QModelIndex(ivle->rootIndex()).internalPointer());
    state["name"] = root->data(0).toString();
    state["check"] = root->data(Qt::CheckStateRole).toInt();
    QJsonArray temp;
    for (int i = 0; i < root->childCount(); i++)
        addChild(root->child(i), temp);
    state["children"] = temp;
    state["files"] = getFiles(root);
    return state;
}

QJsonArray MainWindow::getFiles(WorkbinsViewItem *node)
{
    QJsonArray retVal;
    FileListModel* model = node->fileViewModel();
    for (int i = 0; i < model->rowCount(); i++) {
        QJsonObject row = model->getFile(i), file;
        file["id"] = row["id"];
        file["check"] = row["checked"];
        retVal.append(file);
    }
    return retVal;
}

void MainWindow::refreshFileView(QModelIndex index)
{
    ui->fileListView->setModel(static_cast<WorkbinsViewItem*>(index.internalPointer())->fileViewModel());
}

void MainWindow::syncCompleted() {
    ui->statusBar->showMessage("In Sync");
    ui->syncButton->setVisible(true);
    ui->progressBar->setVisible(false);
    ui->browseButton->setDisabled(false);
}

void MainWindow::loadCompleted()
{
    ui->workbinsView->setModel(ivle->workbinsModel());
    ui->workbinsView->expand(QModelIndex(ivle->rootIndex()));
    ui->statusBar->showMessage("Modules Loaded");
    ui->syncButton->setDisabled(false);
    ui->browseButton->setDisabled(false);
}

void MainWindow::initIVLELoginDlg() {
    IVLELoginDialog* dlg = new IVLELoginDialog(this);
    connect(dlg, SIGNAL(newTokenReceived(QString)), this, SLOT(newTokenReceived(QString)));
    dlg->exec();
    dlg->close();
}

void MainWindow::newTokenReceived(QString newToken) {
    qobject_cast<QDialog*>(sender())->close();
    ivle->token(newToken);
}

void MainWindow::on_browseButton_clicked()
{
    QString newDir = QFileDialog::getExistingDirectory(this, tr("Set Download Location"),"/",
                                                           QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks)+"/";
    qDebug() << newDir;
    DirMover* mover = new DirMover(QDir(man->dir()), QDir(newDir), this);
    mover->startCopy();

    man->changeDownloadDirTo(newDir);
}

void MainWindow::syncProgressChange(int val)
{
    if (val==100)
        syncCompleted();
    ui->progressBar->setValue(val<10?10:val);
}

void MainWindow::fileOpener(QModelIndex index)
{
    QString path = getFilePath(index);
    if (QFile::exists(path))
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    else
        showErrMsg();
}
