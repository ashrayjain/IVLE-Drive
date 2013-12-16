#include "ivleconnector.h"
#include "ivlelogindialog.h"

using namespace std;

QString IVLEConnector::API_KEY = "R7o7vzmqBA3BAXxPrLLD";

IVLEConnector::IVLEConnector(QString _token, QObject *parent) :
    token(_token),
    QObject(parent)
{
    if(token=="" || !isConnectionValid()) {
        IVLELoginDialog* loginDlg = new IVLELoginDialog(dynamic_cast<QWidget*>(parent));
        connect(loginDlg, SIGNAL(urlChanged(QWebView*)), this, SLOT(handleUrlChange(QWebView*)));
        loginDlg->show();
    }
}

bool IVLEConnector::isConnectionValid() {
    QNetworkReply *reply = QNetworkAccessManager().get(QNetworkRequest(QUrl("https://ivle.nus.edu.sg/api/Lapi.svc/Validate?APIKey="+API_KEY+"&Token="+token)));

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    loop.exec();

    QString replyStr(reply->readAll());
    QJsonObject obj = Parser::jsonify(replyStr);

    if (obj["Success"].toBool() == true) {
        token = obj["Token"].toString();
        return true;
    }
    return false;
}

void IVLEConnector::handleUrlChange(QWebView * webView) {
    QUrl baseUrl = "https://ivle.nus.edu.sg/api/login/?apikey=nR7o7vzmqBA3BAXxPrLLD";
    if (webView->url() != baseUrl) {
        qDebug() << webView->page()->mainFrame()->toPlainText();
        webView->setHtml("<h2>Login Successful!</h2>", baseUrl);
    }
}
