#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
class MainWindow;
}

class IVLEConnector;

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

public slots:
    void syncCompleted();

private:
    Ui::MainWindow *ui;
    IVLEConnector* ivle;
    void initIVLELoginDlg();
};

#endif // MAINWINDOW_H
