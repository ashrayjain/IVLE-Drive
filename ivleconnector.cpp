#include <QtNetwork>
#include "parser.h"
#include "ivleconnector.h"
#include "ivlelogindialog.h"
#include "storage.h"

using namespace std;

QString IVLEConnector::API_KEY = "nR7o7vzmqBA3BAXxPrLLD";

IVLEConnector::IVLEConnector(QObject *parent) :
    token(Storage::readToken()),
    QObject(parent),
    status(INVALID_TOKEN)
{    
}

bool IVLEConnector::getWorkbins() {
    qDebug() << "GetRequest\n";
    return status==VALID_TOKEN;
}

void IVLEConnector::initiateTokenValidation() {
    if(token=="")
        emit tokenChanged(false);
    else {
        qDebug() << "trying to validate..";
        QNetworkAccessManager nam;
        QNetworkReply *reply = nam.get(QNetworkRequest(QUrl("https://ivle.nus.edu.sg/api/Lapi.svc/Validate?APIKey="+API_KEY+"&Token="+token)));
        QObject::connect(reply, SIGNAL(finished()), this, SLOT(test()));
    }
}

void IVLEConnector::test() {
    qDebug() << "ttest";
}

void IVLEConnector::setToken(QString newToken) {
    token = newToken;
    status = VALID_TOKEN;
    Storage::writeToken(token);
    qDebug() << "new token found \n";
    emit tokenChanged(true);
}

void IVLEConnector::completedTokenValidation() {
    QJsonObject obj = Parser::jsonify(dynamic_cast<QNetworkReply*>(sender())->readAll());
    if (obj["Success"].toBool() == true) {
        setToken(obj["Token"].toString());
    }
    else
        emit tokenChanged(false);
}
