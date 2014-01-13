#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
class MainWindow;
}

class IVLEConnector;
class DownloadsManager;
class WorkbinsViewItem;
class QProgressBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotTokenChanged(bool tokenValidity);
    void newTokenReceived(QString newToken);
    void on_syncButton_clicked();
    void refreshFileView(QModelIndex index);
    void on_browseButton_clicked();
    void syncProgressChange(int);
    void fileOpener(QModelIndex index);

public slots:
    void syncCompleted();
    void loadCompleted();

private:
    Ui::MainWindow *ui;
    IVLEConnector* ivle;
    DownloadsManager* man;
    void initIVLELoginDlg();
    void loadModules();
    void syncModules();
    void syncModule(WorkbinsViewItem* );
    void syncFolder(WorkbinsViewItem* );
    void syncFile(QJsonObject &file);
    QString getFilePath(QModelIndex index);
    void showErrMsg();
    void addChild(WorkbinsViewItem* node, QJsonArray &arr);
    QJsonObject getState();
    QJsonArray getFiles(WorkbinsViewItem* node);
};

#endif // MAINWINDOW_H
