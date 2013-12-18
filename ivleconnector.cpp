#include <QtNetwork>
#include "parser.h"
#include "ivleconnector.h"
#include "ivlelogindialog.h"
#include "storage.h"

using namespace std;

QString IVLEConnector::API_KEY      = "nR7o7vzmqBA3BAXxPrLLD";
QString IVLEConnector::INVALID_ID   = "00000000-0000-0000-0000-000000000000";

IVLEConnector::IVLEConnector(QObject *parent) :
    token(Storage::readToken()),
    QObject(parent),
    status(INVALID_TOKEN),
    nam(new QNetworkAccessManager())
{    
}

void IVLEConnector::syncWorkbins() {
    qDebug() << "I'm Syncing Workbins";
    if (status==VALID_TOKEN) {
        QUrl url("https://ivle.nus.edu.sg/api/Lapi.svc/Modules_Student?APIKey="+API_KEY+"&AuthToken="+token+"&IncludeAllInfo=false");
        networkConnect(url, this, SLOT(populateCourseIDs()));
    }
    else
        emit tokenChanged(false);
}

void IVLEConnector::populateCourseID(QJsonObject result)
{
    QString id = result["ID"].toString();
    if (id != INVALID_ID) {
        QMap<QString, QString> info;
        info["Code"] = result["CourseCode"].toString();
        info["Name"] = result["CourseName"].toString();
        data[id] = info;
        qDebug()<<data;
    }
}

void IVLEConnector::populateCourseIDs() {
    QJsonObject obj = Parser::jsonify(qobject_cast<QNetworkReply*>(sender())->readAll());
    sender()->deleteLater();
    if (obj["Comments"].toString() == "Invalid login!")
        emit tokenChanged(false);
    else {
        QJsonArray results = obj["Results"].toArray();
        for (QJsonArray::const_iterator it = results.constBegin(); it != results.constEnd(); it++)
            populateCourseID((*it).toObject());
    }
    /*
    QStandardItemModel* model = new QStandardItemModel(this);
    QStandardItem *parentItem = model->invisibleRootItem();
    for (int i = 0; i < 4; ++i) {
        QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
        item->setCheckable(true);
        item->setCheckState(Qt::Unchecked);
        parentItem->appendRow(item);
        parentItem = item;
    }
    */
}

void IVLEConnector::initiateTokenValidation() {
    if(token=="")
        emit tokenChanged(false);
    else
        networkConnect("https://ivle.nus.edu.sg/api/Lapi.svc/Validate?APIKey="+API_KEY+"&Token="+token,
                       this,
                       SLOT(completedTokenValidation()));
}

void IVLEConnector::networkConnect(const QUrl &url, const QObject *receiver, const char* slot) {
    QNetworkReply *reply = nam->get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(readyRead()), receiver, slot);
}

void IVLEConnector::setToken(QString newToken) {
    token = newToken;
    status = VALID_TOKEN;
    Storage::writeToken(token);
    emit tokenChanged(true);
}

void IVLEConnector::completedTokenValidation() {
    QJsonObject obj = Parser::jsonify(qobject_cast<QNetworkReply*>(sender())->readAll());
    sender()->deleteLater();
    if (obj["Success"].toBool() == true)
        setToken(obj["Token"].toString());
    else
        emit tokenChanged(false);
}

IVLEConnector::~IVLEConnector() {
    delete nam;
}
