#ifndef IVLELOGINDIALOG_H
#define IVLELOGINDIALOG_H

#include <QDialog>
#include <QtWebKitWidgets>

namespace Ui {
class IVLELoginDialog;
}

class IVLELoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IVLELoginDialog(QWidget *parent = 0);
    ~IVLELoginDialog();

private slots:
    void handleUrlChange(bool ok);

signals:
    void urlChanged(QWebView* webView);

private:
    Ui::IVLELoginDialog *ui;
};

#endif // IVLELOGINDIALOG_H
