#include "filedownloader.h"

QString FileDownloader::API_KEY     = "nR7o7vzmqBA3BAXxPrLLD";
QString FileDownloader::baseUrl     = "https://ivle.nus.edu.sg/api/downloadfile.ashx?";

FileDownloader::FileDownloader(QString fileName, QObject *parent):
    QObject(parent),
    fileName(fileName)
{
}

void FileDownloader::startDownload(QString token, QString id)
{
    QUrl url = baseUrl+"APIKey="+API_KEY+"&AuthToken="+token+"&ID="+id+"&target=workbin";
    QNetworkReply* reply = nam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(readyRead()), this, SLOT(bytesReady()));
    connect(reply, SIGNAL(finished()), this, SLOT(downloadComplete()));
}

void FileDownloader::bytesReady()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    buffer.append(reply->read(reply->bytesAvailable()));
}

void FileDownloader::downloadComplete()
{
    file.setFileName(fileName);
    if (!(file.exists() || !file.open(QIODevice::WriteOnly)))
        file.write(buffer);
    file.close();
    emit fileDownloaded();
}

