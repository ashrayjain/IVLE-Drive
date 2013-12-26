#include <QtNetwork>
#include "parser.h"
#include "ivleconnector.h"
#include "ivlelogindialog.h"
#include "storage.h"
#include "filedownloader.h"


using namespace std;

QString IVLEConnector::API_KEY      = "nR7o7vzmqBA3BAXxPrLLD";
QString IVLEConnector::INVALID_ID   = "00000000-0000-0000-0000-000000000000";
QString IVLEConnector::baseUrl      = "https://ivle.nus.edu.sg/api/Lapi.svc/";

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
        QUrlQuery query;
        query.addQueryItem("APIKey", API_KEY);
        query.addQueryItem("AuthToken", token);
        query.addQueryItem("IncludeAllInfo", "false");
        networkConnect("Modules_Student", query, this, SLOT(fetchModules()));
    }
    else
        emit tokenChanged(false);
}

WorkbinsViewModel *IVLEConnector::workbinsModel()
{
    return data;
}

QModelIndex& IVLEConnector::rootIndex()
{
    return data->rootIndex().child(0,0);
}

void IVLEConnector::initiateFetchModuleInfo(QJsonObject result)
{
    QUrlQuery query;
    query.addQueryItem("APIKey",        API_KEY);
    query.addQueryItem("AuthToken",     token);
    query.addQueryItem("CourseID",      result["ID"].toString());
    query.addQueryItem("CourseCode",    result["CourseCode"].toString());
    query.addQueryItem("CourseName",    result["CourseName"].toString());

    networkConnect("Workbins", query, this, SLOT(fetchModuleInfo()));
}

void IVLEConnector::addModuleToModel(WorkbinsViewItem* node, QJsonObject &workbin)
{
    WorkbinsViewItem *child = new WorkbinsViewItem();
    addDataToItem(child, workbin["Title"].toString(), workbin["Title"].toString());
    node->appendChild(child);

    QJsonArray folders = workbin["Folders"].toArray();
    for (QJsonArray::const_iterator it = folders.constBegin(); it != folders.constEnd(); it++)
        addFolderToModel(child, (*it).toObject());
}

void IVLEConnector::addFolderToModel(WorkbinsViewItem *node, QJsonObject &folder)
{
    WorkbinsViewItem *child = new WorkbinsViewItem();
    addDataToItem(child, folder["FolderName"].toString(), folder["FolderName"].toString());
    node->appendChild(child);

    QJsonArray folders = folder["Folders"].toArray();
    for (QJsonArray::const_iterator it = folders.constBegin(); it != folders.constEnd(); it++)
        addFolderToModel(child, (*it).toObject());

    QJsonArray files = folder["Files"].toArray();
    QList<QJsonObject> filesData;
    for (QJsonArray::const_iterator it = files.constBegin(); it != files.constEnd(); it++)
        addFileToModel(filesData, (*it).toObject());

    addDataToItem(child, folder["FolderName"].toString(), folder["FolderName"].toString(), filesData);
}

void IVLEConnector::addFileToModel(QList<QJsonObject> &node, QJsonObject &file)
{
    QJsonObject fileData;
    fileData["name"] = file["FileName"].toString();
    fileData["id"] = file["ID"].toString();
    fileData["type"] = file["FileType"].toString();
    FileDownloader* t = new FileDownloader(token, file["ID"].toString(), file["FileName"].toString(), this);
    node.append(fileData);
}

void IVLEConnector::addDataToItem(WorkbinsViewItem *node, QString &code, QString &name, QList<QJsonObject> &data)
{
    node->appendData(QVariant(code));
    node->appendData(QVariant(name));
    node->setFileData(data);
}

void IVLEConnector::fetchModules() {
    QJsonObject obj = Parser::jsonify(qobject_cast<QNetworkReply*>(sender())->readAll());
    sender()->deleteLater();
    if (obj["Comments"].toString() == "Invalid login!")
        emit tokenChanged(false);
    else {
        QJsonArray results = obj["Results"].toArray();
        noOfModules = results.size();

        WorkbinsViewItem* rootNode = new WorkbinsViewItem();
        addDataToItem(rootNode, QString("My Modules"), QString(""));
        root->appendChild(rootNode);
        root = rootNode;

        for (QJsonArray::const_iterator it = results.constBegin(); it != results.constEnd(); it++)
            initiateFetchModuleInfo((*it).toObject());
    }
}

void IVLEConnector::fetchModuleInfo()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QUrlQuery query(reply->request().url().query());

    WorkbinsViewItem* module = new WorkbinsViewItem();
    addDataToItem(module, query.queryItemValue("CourseCode"), query.queryItemValue("CourseName"));
    root->appendChild(module);

    QJsonObject obj = Parser::jsonify(reply->readAll());
    sender()->deleteLater();

    QJsonArray results = obj["Results"].toArray();
    for (QJsonArray::const_iterator it = results.constBegin(); it != results.constEnd(); it++)
        addModuleToModel(module, (*it).toObject());

    noOfModules--;
    if (noOfModules == 0)
        emit syncCompleted();
}

void IVLEConnector::initiateTokenValidation() {
    if(token=="")
        emit tokenChanged(false);
    else {
        QUrlQuery query;
        query.addQueryItem("APIKey", API_KEY);
        query.addQueryItem("Token", token);
        networkConnect("Validate", query, this, SLOT(completedTokenValidation()));

    }
}

void IVLEConnector::networkConnect(const QString &path, const QUrlQuery &params, const QObject *receiver, const char* slot) {
    QUrl url(baseUrl+path);
    url.setQuery(params);
    QNetworkReply *reply = nam->get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), receiver, slot);
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
