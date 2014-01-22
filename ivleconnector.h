#ifndef IVLECONNECTOR_H
#define IVLECONNECTOR_H

#include <QObject>
#include <QtNetwork>
#include "workbinsviewitem.h"
#include "workbinsviewmodel.h"

class DownloadsManager;

class IVLEConnector : public QObject
{
    Q_OBJECT
private:
    enum STATUS { VALID_TOKEN, INVALID_TOKEN };

    static QString API_KEY;
    static QString INVALID_ID;
    static QString baseUrl;

    QString _token;
    STATUS status;
    QNetworkAccessManager *nam;
    WorkbinsViewModel* data;
    WorkbinsViewItem* root;
    int noOfModules;
    QJsonObject prevState;

    void networkConnect(const QString &path, const QUrlQuery &params, const QObject*, const char*);
    void initiateFetchModuleInfo(QJsonObject result);
    void addModuleToModel(WorkbinsViewItem* node, QJsonObject &workbin, QJsonObject &prevModState);
    void addFolderToModel(WorkbinsViewItem* node, QJsonObject &folder, QJsonObject &prevFolderState = QJsonObject());
    void addFileToModel(QList<QJsonObject>& node, QJsonObject &file, bool prevFileState = true);
    void addDataToItem(WorkbinsViewItem* node, QString &code, QString &name, QList<QJsonObject> &data = QList<QJsonObject>());
    int retrieveModuleState(QString code);

public slots:
    void initiateTokenValidation();
    void completedTokenValidation();

private slots:
    void fetchModules();
    void fetchModuleInfo();

signals:
    void tokenChanged(bool tokenValidity);
    void loadCompleted();

public:
    explicit IVLEConnector(QObject *parent = 0);
    void token(QString newToken);
    QString token();
    void loadModules();
    WorkbinsViewModel* workbinsModel();
    QModelIndex& rootIndex();
    ~IVLEConnector();
};

#endif // IVLECONNECTOR_H

