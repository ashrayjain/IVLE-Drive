#include "ivlelogindialog.h"
#include "ui_ivlelogindialog.h"

IVLELoginDialog::IVLELoginDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::IVLELoginDialog)
{
    ui->setupUi(this);
}

IVLELoginDialog::~IVLELoginDialog()
{
    delete ui;
}

void IVLELoginDialog::handleUrlChange(bool ok) {
    QWebView* webView = dynamic_cast<QWebView*>(sender());
    if (ok) {
        emit this->urlChanged(webView);
    }

    else {

    }
}
