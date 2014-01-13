#include <QtNetwork>
#include "parser.h"
#include "ivleconnector.h"
#include "ivlelogindialog.h"
#include "storage.h"
#include "downloadsmanager.h"


using namespace std;

QString IVLEConnector::API_KEY      = "nR7o7vzmqBA3BAXxPrLLD";
QString IVLEConnector::INVALID_ID   = "00000000-0000-0000-0000-000000000000";
QString IVLEConnector::baseUrl      = "https://ivle.nus.edu.sg/api/Lapi.svc/";

IVLEConnector::IVLEConnector(QObject *parent) :
    _token(Storage::readToken()),
    QObject(parent),
    status(INVALID_TOKEN),
    nam(new QNetworkAccessManager()),
    data(new WorkbinsViewModel()),
    root(data->invisibleRootItem()),
    prevState(Storage::loadState())
{
}

void IVLEConnector::loadModules() {
    qDebug() << "I'm Loading Modules";
    if (status==VALID_TOKEN) {
        QUrlQuery query;
        query.addQueryItem("APIKey", API_KEY);
        query.addQueryItem("AuthToken", _token);
        query.addQueryItem("IncludeAllInfo", "false");
        networkConnect("Modules_Student", query, this, SLOT(fetchModules()));
    }
    else
        emit tokenChanged(false);
}

void IVLEConnector::fetchModules() {
    QJsonObject obj = Parser::jsonify(qobject_cast<QNetworkReply*>(sender())->readAll());
    sender()->deleteLater();
    if (obj["Comments"].toString() == "Invalid login!")
        emit tokenChanged(false);
    else {
        QJsonArray results = obj["Results"].toArray();
        noOfModules = results.size();

        WorkbinsViewItem* rootNode;
        if (prevState["name"].toString()=="My Modules")
            rootNode = new WorkbinsViewItem(prevState["check"].toDouble());
        else
            rootNode = new WorkbinsViewItem();

        addDataToItem(rootNode, QString("My Modules"), QString(""));
        root->appendChild(rootNode);
        root = rootNode;

        for (QJsonArray::const_iterator it = results.constBegin(); it != results.constEnd(); it++)
            initiateFetchModuleInfo((*it).toObject());
    }
}

void IVLEConnector::initiateFetchModuleInfo(QJsonObject result)
{
    QUrlQuery query;
    query.addQueryItem("APIKey",        API_KEY);
    query.addQueryItem("AuthToken",     _token);
    query.addQueryItem("CourseID",      result["ID"].toString());
    query.addQueryItem("CourseCode",    result["CourseCode"].toString());
    query.addQueryItem("CourseName",    result["CourseName"].toString());

    networkConnect("Workbins", query, this, SLOT(fetchModuleInfo()));
}

void IVLEConnector::fetchModuleInfo()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QUrlQuery query(reply->request().url().query());

    int i = retrieveModuleState(query.queryItemValue("CourseCode"));
    QJsonObject mod;
    WorkbinsViewItem* module;
    qDebug() << i;
    if (i != -1) {
        mod = prevState["children"].toArray()[i].toObject();
        qDebug() << mod["check"].toDouble();
        module = new WorkbinsViewItem(mod["check"].toDouble());
    }
    else
        module = new WorkbinsViewItem();

    addDataToItem(module, query.queryItemValue("CourseCode"), query.queryItemValue("CourseName"));
    root->appendChild(module);

    QJsonObject obj = Parser::jsonify(reply->readAll());
    sender()->deleteLater();

    QJsonArray results = obj["Results"].toArray();
    for (QJsonArray::const_iterator it = results.constBegin(); it != results.constEnd(); it++)
        addModuleToModel(module, (*it).toObject(), mod);

    noOfModules--;
    if (noOfModules == 0)
        emit loadCompleted();
}

void IVLEConnector::addModuleToModel(WorkbinsViewItem* node, QJsonObject &workbin, QJsonObject &mod)
{
    //qDebug() << mod;
    WorkbinsViewItem * child;
//    if (mod.empty())
        child = new WorkbinsViewItem();
//    else {
//        QJsonArray children = mod["children"].toArray();
//        for (QJsonArray::const_iterator it = children.constBegin(); it != children.constEnd(); it++)
//            if ((*it).toObject()["name"].toString() == workbin["Title"].toString()) {
//                child = new WorkbinsViewItem((*it).toObject()["check"].toDouble());
//                break;
//            }
//    }

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
    fileData["checked"] = true;
    node.append(fileData);
}

void IVLEConnector::addDataToItem(WorkbinsViewItem *node, QString &code, QString &name, QList<QJsonObject> &data)
{
    node->appendData(QVariant(code));
    node->appendData(QVariant(name));
    node->setFileData(data);
}

int IVLEConnector::retrieveModuleState(QString code)
{
    int ret = -1;
    if (prevState["children"].isArray()) {
        QJsonArray modules = prevState["children"].toArray();
        QJsonObject modState = modules[0].toObject();
        for (ret = 1; ret <= modules.size() && code != modState["name"].toString(); ret++) {
            qDebug() << modState["name"].toString();
            modState = modules[ret].toObject();
        }
        if (ret > modules.size())
            ret = -1;
    }
    qDebug() << code << ret;
    return ret;
}

void IVLEConnector::initiateTokenValidation() {
    if(_token=="")
        emit tokenChanged(false);
    else {
        QUrlQuery query;
        query.addQueryItem("APIKey", API_KEY);
        query.addQueryItem("Token", _token);
        networkConnect("Validate", query, this, SLOT(completedTokenValidation()));

    }
}

void IVLEConnector::networkConnect(const QString &path, const QUrlQuery &params, const QObject *receiver, const char* slot) {
    QUrl url(baseUrl+path);
    url.setQuery(params);
    QNetworkReply *reply = nam->get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), receiver, slot);
}

void IVLEConnector::token(QString newToken) {
    _token = newToken;
    status = VALID_TOKEN;
    Storage::writeToken(_token);
    emit tokenChanged(true);
}

QString IVLEConnector::token()
{
    return _token;
}

void IVLEConnector::completedTokenValidation() {
    QJsonObject obj = Parser::jsonify(qobject_cast<QNetworkReply*>(sender())->readAll());
    sender()->deleteLater();
    if (obj["Success"].toBool() == true)
        token(obj["Token"].toString());
    else
        emit tokenChanged(false);
}

IVLEConnector::~IVLEConnector() {
    delete nam;
}

WorkbinsViewModel *IVLEConnector::workbinsModel() {
    return data;
}

QModelIndex& IVLEConnector::rootIndex() {
    return data->rootIndex();
}
