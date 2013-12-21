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

    QString token;
    STATUS status;
    QNetworkAccessManager *nam;
    WorkbinsViewModel* data;
    WorkbinsViewItem* root;

    void networkConnect(const QUrl&, const QObject*, const char*);
    void populateCourseID(QJsonObject result);

public slots:
    void initiateTokenValidation();
    void completedTokenValidation();
    void populateCourseIDs();

signals:
    void tokenChanged(bool tokenValidity);
    void syncCompleted();

public:
    explicit IVLEConnector(QObject *parent = 0);
    void setToken(QString newToken);
    void syncWorkbins();
    WorkbinsViewModel* workbinsModel();
    ~IVLEConnector();
};

#endif // IVLECONNECTOR_H

