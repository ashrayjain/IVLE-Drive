#ifndef IVLECONNECTOR_H
#define IVLECONNECTOR_H

#include <QObject>
#include <QtNetwork>
#include "workbinsviewitem.h"
#include "workbinsviewmodel.h"

class IVLEConnector : public QObject
{
    Q_OBJECT
private:
    enum STATUS { VALID_TOKEN, INVALID_TOKEN };

    static QString API_KEY;
    static QString INVALID_ID;
    static QString baseUrl;

    QString token;
    STATUS status;
    QNetworkAccessManager *nam;
    WorkbinsViewModel* data;
    WorkbinsViewItem* root;
    int noOfModules;

    void networkConnect(const QString &path, const QUrlQuery &params, const QObject*, const char*);
    void initiateFetchModuleInfo(QJsonObject result);
    void addModuleToModel(WorkbinsViewItem* node, QJsonObject &workbin);
    void addFolderToModel(WorkbinsViewItem* node, QJsonObject &folder);
    void addFileToModel(QList<QJsonObject>& node, QJsonObject &file);
    void addDataToItem(WorkbinsViewItem* node, QString &code, QString &name, QList<QJsonObject> &data = QList<QJsonObject>());

public slots:
    void initiateTokenValidation();
    void completedTokenValidation();

private slots:
    void fetchModules();
    void fetchModuleInfo();

signals:
    void tokenChanged(bool tokenValidity);
    void syncCompleted();

public:
    explicit IVLEConnector(QObject *parent = 0);
    void setToken(QString newToken);
    void syncWorkbins();
    WorkbinsViewModel* workbinsModel();
    QModelIndex& rootIndex();
    ~IVLEConnector();
};

#endif // IVLECONNECTOR_H

