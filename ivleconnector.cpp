#include <QtNetwork>
#include "parser.h"
#include "ivleconnector.h"
#include "ivlelogindialog.h"
#include "storage.h"


using namespace std;

QString IVLEConnector::API_KEY      = "nR7o7vzmqBA3BAXxPrLLD";
QString IVLEConnector::INVALID_ID   = "0000000-0000-0000-0000-000000000000";

IVLEConnector::IVLEConnector(QObject *parent) :
    token(Storage::readToken()),
    QObject(parent),
    status(INVALID_TOKEN),
    nam(new QNetworkAccessManager()),
    data(new WorkbinsViewModel()),
    root(data->invisibleRootItem())
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

WorkbinsViewModel *IVLEConnector::workbinsModel()
{
    return data;
}

void IVLEConnector::populateCourseID(QJsonObject result)
{
    QList<QVariant> test;
    test.append("No Workbins Here! :)");
    QString id = result["ID"].toString();
    if (id != INVALID_ID) {
        QList<QVariant> data;
        data.append(result["CourseCode"].toString()+" "+ result["CourseName"].toString());
        WorkbinsViewItem* child = new WorkbinsViewItem(data);
        root->appendChild(child);
        child->appendChild(new WorkbinsViewItem(test));
        child->appendChild(new WorkbinsViewItem(test));
        child->appendChild(new WorkbinsViewItem(test));
        qDebug() << data;
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
        emit syncCompleted();
    }
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
