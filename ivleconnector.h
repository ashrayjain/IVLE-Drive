#ifndef IVLECONNECTOR_H
#define IVLECONNECTOR_H

#include <QObject>

class IVLEConnector : public QObject
{
    Q_OBJECT
private:
    enum STATUS { VALID_TOKEN, INVALID_TOKEN };

    static QString API_KEY;
    QString token;
    STATUS status;

public slots:
    void initiateTokenValidation();
    void completedTokenValidation();
    void test();

signals:
    void tokenChanged(bool tokenValidity);

public:
    explicit IVLEConnector(QObject *parent = 0);
    void setToken(QString newToken);
    bool getWorkbins();
};

#endif // IVLECONNECTOR_H
