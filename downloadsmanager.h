#ifndef DOWNLOADSMANAGER_H
#define DOWNLOADSMANAGER_H

#include <QObject>
#include <QDir>
#include <QVector>
#include <QPair>

class DownloadsManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadsManager(QObject *parent = 0);
    ~DownloadsManager();
    void initManager();
    void changeDownloadDirTo(QString newDir);
    void changeToDir(QString folderName);
    void removeDir(QString dirName);
    void removeFile(QString fileName);
    void stackDownload(QString id, QString fileName);
    void executeDownloads(QString token);
    void upDir();
    QString dir();


signals:
    void downloadDirChanged(QString dir);
    void progress(int);

public slots:
    void fileDownloaded();

private:
    QDir downloadDir;
    QVector<QPair<QString, QString>> downloads;
    int numDownloads, downloadsDone;
    void downloadFile(QString token, QString id, QString fileName);
};

#endif // DOWNLOADSMANAGER_H
