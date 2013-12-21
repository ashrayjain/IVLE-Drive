#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ivleconnector.h"
#include "ivlelogindialog.h"
#include "filelistmodel.h"
#include "filelistdelegate.h"
#include "workbinsviewitem.h"
#include "workbinsviewmodel.h"


// Green - #98FF75

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ivle(new IVLEConnector(this))
{
    ui->setupUi(this);
    //ui->syncButton->setDisabled(true);
    ui->statusBar->showMessage("Trying to connect with IVLE..");
    connect(ivle, SIGNAL(tokenChanged(bool)), this, SLOT(slotTokenChanged(bool)));
    ivle->initiateTokenValidation();
    QList<QVariant> temp;
//    temp.append(QString("test"));
//    temp.append(QString("testdesc"));
//    WorkbinsViewModel* model = new WorkbinsViewModel(this);
//    WorkbinsViewItem* root = model->invisibleRootItem();
//    root->appendChild(new WorkbinsViewItem(temp));
//    root->appendChild(new WorkbinsViewItem(temp));
//    root->appendChild(new WorkbinsViewItem(temp));
//    root->child(1)->appendChild(new WorkbinsViewItem(temp));
//    ui->workbinsView->setModel(model);
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
    connect(ivle, SIGNAL(syncCompleted()), this, SLOT(syncCompleted()));
    ivle->syncWorkbins();


//    FileListModel* model = new FileListModel(this);
//    QVector<QJsonObject> test;
//    QJsonObject tmp;
//    tmp["id"] = QString("1");
//    tmp.insert("id", QString("1"));
//    tmp.insert("name", QString("test"));
//    tmp.insert("desc", QString("testDesc"));
//    test.append(tmp);
//    tmp.insert("id", QString("2"));
//    tmp.insert("name", QString("testxbvcbcbxcvbxvcbcxvbxcvbcvbcvxbcvbcvbcvbcvb2"));
//    tmp.insert("desc", QString("testDesc2"));
//    test.append(tmp);
//    qDebug() << test;
//    model->setFileList(test);
//    ui->fileListView->setModel(model);
//    FileListDelegate* deletegate = new FileListDelegate(this);
//    ui->fileListView->setItemDelegate(deletegate);
    //ui->treeView->header()->model()->setHeaderData(0, Qt::Orientation::Horizontal, QVariant("List of Workbins"));
//    QStandardItemModel* model = new QStandardItemModel(this);
//    QStandardItem *parentItem = model->invisibleRootItem();
//    for (int i = 0; i < 4; ++i) {
//        QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
//        item->setCheckable(true);
//        item->setCheckState(Qt::Unchecked);
//        parentItem->appendRow(item);
//        parentItem = item;
//        QStandardItem *item2 = new QStandardItem(QString("item %0").arg(i));
//        QStandardItem *item3 = new QStandardItem(QString("item %0").arg(i));
//        parentItem->appendRow(item2);
//        parentItem->appendRow(item3);
//        parentItem = model->invisibleRootItem();
//    }
//    QStandardItemModel* model = new QStandardItemModel(this);
//    QStandardItem *parentItem = model->invisibleRootItem();
//    for (int i = 0; i < 4; ++i) {
//        Module* item = new Module();
//        item->id(QString("test"));
//        item->name(QString("testname"));
//        item->setCheckable(true);
//        item->setCheckState(Qt::Unchecked);
//        parentItem->appendRow(item);
//        parentItem = item;
//    }
//    qDebug() << "Coptes";
//    model->setItemPrototype(new Module());
//    QList<QVariant> temp;
//    temp.append(QString("test42435"));
//    temp.append(QString("testdesc"));
//    WorkbinsViewModel* model  = qobject_cast<WorkbinsViewModel*>(ui->workbinsView->model());
//    WorkbinsViewItem* root = model->invisibleRootItem();
//    root->appendChild(new WorkbinsViewItem(temp));
//    ui->workbinsView->setModel(model);

    //double i = 0;
    //while(i<1000000000)
    //    i++;
    //root->appendChild(new WorkbinsViewItem(temp));
    //root->appendChild(new WorkbinsViewItem(temp));
    //connect(ui->workbinsView, SIGNAL(clicked(QModelIndex)),);
//  ui->workbinsView->setItemDelegate(deletegate);

}

void MainWindow::syncCompleted() {
    ui->workbinsView->setModel(ivle->workbinsModel());
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
