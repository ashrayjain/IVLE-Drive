#include "ivleconnector.h"
#include "ivlelogindialog.h"

using namespace std;

QString IVLEConnector::API_KEY = "nR7o7vzmqBA3BAXxPrLLD";

IVLEConnector::IVLEConnector(QString _token, QObject *parent) :
    token(_token),
    QObject(parent)
{
    //connect(this, SIGNAL(tokenChange(QString)), parent, SLOT(slotTokenChange(QString)));
    if(token=="" || !isConnectionValid()) {
        status = INVALID_TOKEN;
        //IVLELoginDialog* loginDlg = new IVLELoginDialog(dynamic_cast<QWidget*>(parent));
        //connect(loginDlg, SIGNAL(urlChanged(QWebView*)), this, SLOT(handleUrlChange(QWebView*)));
        //loginDlg->exec();
        //loginDlg->close();
    }
    else
        status = VALID_TOKEN;
}

bool IVLEConnector::isConnectionValid() {
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.get(QNetworkRequest(QUrl("https://ivle.nus.edu.sg/api/Lapi.svc/Validate?APIKey="+API_KEY+"&Token="+token)));

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString replyStr(reply->readAll());
    QJsonObject obj = Parser::jsonify(replyStr);

    if (obj["Success"].toBool() == true) {
        token = obj["Token"].toString();
        return true;
    }
    return false;
}

bool IVLEConnector::getWorkbins() {
    return status==VALID_TOKEN;
}

void IVLEConnector::handleUrlChange(QWebView * webView) {
    QUrl baseUrl = "https://ivle.nus.edu.sg/api/login/?apikey=nR7o7vzmqBA3BAXxPrLLD";
    if (webView->url() != baseUrl) {
        token = webView->page()->mainFrame()->toPlainText();
        emit tokenChange(token);
        dynamic_cast<QDialog*>(webView->parent())->close();
    }
}
