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
    QUrl baseUrl = "https://ivle.nus.edu.sg/api/login/?apikey=nR7o7vzmqBA3BAXxPrLLD";
    if (ok) {
        if (webView->url() != baseUrl)
            emit tokenChanged(webView->page()->mainFrame()->toPlainText());
    }
    else
        webView->setHtml("<h2>Oops! Something's not right with your network!</h2>");
}
