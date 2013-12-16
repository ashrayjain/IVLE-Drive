#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

public slots:
    void slotTokenChange(QString);

private slots:
    void on_actionLogin_triggered();

    void on_syncButton_clicked();

private:
    Ui::MainWindow *ui;
    IVLEConnector* ivle;
};

#endif // MAINWINDOW_H
