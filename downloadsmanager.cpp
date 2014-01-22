#include <QDir>
#include "downloadsmanager.h"
#include "filedownloader.h"
#include "storage.h"

DownloadsManager::DownloadsManager(QObject *parent) :
    QObject(parent)
{
}

DownloadsManager::~DownloadsManager()
{
    Storage::writeDownloadDir(downloadDir.path());
}

void DownloadsManager::initManager()
{
    // TODO: Get Download locaation from storage
    QString dir = Storage::getDownloadDir();
    if (dir!="")
        downloadDir.setPath(dir);
    else
        downloadDir.setPath(downloadDir.currentPath());
    qDebug() << downloadDir.path();
    emit downloadDirChanged(downloadDir.path());
}

void DownloadsManager::changeDownloadDirTo(QString newDir)
{
    downloadDir.setPath(newDir);
    emit downloadDirChanged(downloadDir.path());
}

void DownloadsManager::downloadFile(QString token, QString id, QString fileName)
{
    if (!downloadDir.exists(fileName)) {
        FileDownloader* t = new FileDownloader(fileName, this);
        connect(t, SIGNAL(fileDownloaded()), this, SLOT(fileDownloaded()));
        t->startDownload(token, id);
    }
    else fileDownloaded();
}

void DownloadsManager::changeToDir(QString folderName)
{
    if (!downloadDir.cd(folderName)) {
        downloadDir.mkdir(folderName);
        downloadDir.cd(folderName);
    }
}

void DownloadsManager::removeDir(QString dirName)
{
    changeToDir(dirName);

    foreach (QString entry, downloadDir.entryList(QDir::NoDotAndDotDot|QDir::Dirs|QDir::Files))
        if (!downloadDir.remove(entry))
            removeDir(entry);

    upDir();

    downloadDir.rmdir(dirName);
}

void DownloadsManager::removeFile(QString fileName)
{
    downloadDir.remove(fileName);
}

void DownloadsManager::stackDownload(QString id, QString fileName)
{
    if (!downloadDir.exists(fileName))
        downloads.append(QPair<QString, QString>(id, downloadDir.path()+"/"+fileName));
}

void DownloadsManager::executeDownloads(QString token)
{
    numDownloads = downloads.size();
    downloadsDone = 0;
    if (numDownloads==0) {
        numDownloads++;
        emit fileDownloaded();
    }
    else
        for (;!downloads.empty();downloads.pop_back())
            downloadFile(token, downloads.last().first, downloads.last().second);
}

void DownloadsManager::upDir()
{
    downloadDir.cdUp();
}

QString DownloadsManager::dir()
{
    return downloadDir.path();
}

void DownloadsManager::fileDownloaded()
{
    downloadsDone++;
    emit progress(downloadsDone*100/numDownloads);
}
