#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QtNetwork>
#include <QFile>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QString token, QString id, QString filename, QObject *parent = 0);

signals:
    void downloaded();

private slots:
    void bytesReady();
    void downloadComplete();

private:
    QNetworkAccessManager nam;
    QFile file;
    static QString baseUrl;
    static QString API_KEY;
};

#endif // FILEDOWNLOADER_H
