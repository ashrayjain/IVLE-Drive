#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QtNetwork>
#include <QFile>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QString fileName, QObject *parent = 0);
    void startDownload(QString token, QString id);

signals:
    void fileDownloaded();

private slots:
    void bytesReady();
    void downloadComplete();

private:
    QNetworkAccessManager nam;
    QFile file;
    QByteArray buffer;
    QString fileName;

    static QString baseUrl;
    static QString API_KEY;
};

#endif // FILEDOWNLOADER_H
