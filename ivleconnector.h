#ifndef IVLECONNECTOR_H
#define IVLECONNECTOR_H

#include <iostream>
#include <string>
#include <QObject>
#include <QtNetwork>
#include "parser.h"

class IVLEConnector : public QObject
{
    Q_OBJECT
private:
    QString token;
    static QString API_KEY;
    bool isConnectionValid();
public:
    explicit IVLEConnector(QString _token, QObject *parent = 0);
};

#endif // IVLECONNECTOR_H
