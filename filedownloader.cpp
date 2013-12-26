#include "filedownloader.h"

QString FileDownloader::API_KEY      = "nR7o7vzmqBA3BAXxPrLLD";
QString FileDownloader::baseUrl = "https://ivle.nus.edu.sg/api/downloadfile.ashx?";

FileDownloader::FileDownloader(QString token, QString id, QString filename, QObject *parent):
    QObject(parent),
    file("/test/"+filename)
{
    if (file.exists() || !file.open(QIODevice::WriteOnly))
        return;
    QUrl url = baseUrl+"APIKey="+API_KEY+"&AuthToken="+token+"&ID="+id+"&target=workbin";
    QNetworkReply* reply = nam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(readyRead()), this, SLOT(bytesReady()));
    connect(reply, SIGNAL(finished()), this, SLOT(downloadComplete()));
}

void FileDownloader::bytesReady()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    file.write(reply->read(reply->bytesAvailable()));
}

void FileDownloader::downloadComplete()
{
    file.close();
    emit downloaded();
}
