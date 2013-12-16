#ifndef IVLECONNECTOR_H
#define IVLECONNECTOR_H

#include <iostream>
#include <string>
#include <QObject>
#include <QtNetwork>
#include <QtWebKitWidgets>
#include "parser.h"

class IVLEConnector : public QObject
{
    Q_OBJECT
private:
    QString token;
    enum STATUS {
        VALID_TOKEN,
        INVALID_TOKEN
    };

    STATUS status;
    static QString API_KEY;
    bool isConnectionValid();

private slots:
    void handleUrlChange(QWebView*);

signals:
    void tokenChange(QString);

public:
    explicit IVLEConnector(QString _token, QObject *parent = 0);
    bool getWorkbins();
};

#endif // IVLECONNECTOR_H
