#include "storage.h"
#include <QFile>
#include <QTextStream>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

QString Storage::readToken() {
    QFile file(".token");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";
    return QTextStream(&file).readLine();
}

void Storage::writeToken(QString &token) {
    QFile file(".token");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return;
    QTextStream(&file)<<token;
    file.close();
}

QIcon Storage::getFileIcon(QString ext) {
    QTemporaryFile file("XXXXXX."+ext);
    if (file.open())
        return QFileIconProvider().icon(QFileInfo(file.fileName()));
    return QIcon();
}

QString Storage::getDownloadDir()
{
    QFile file(".downloadDir");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";
    return QTextStream(&file).readLine();
}

void Storage::writeDownloadDir(QString &dir)
{
    QFile file(".downloadDir");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return;
    QTextStream(&file)<<dir;
    file.close();
}

void Storage::saveState(QJsonObject &state)
{
    QFile file(".binfile");
    if(!file.open(QIODevice::WriteOnly)) return;
    file.write(QJsonDocument(state).toBinaryData());
    file.close();
}

QJsonObject Storage::loadState()
{
    QFile file(".binfile");
    QJsonObject obj;
    if(file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromBinaryData(file.readAll());
        if (!doc.isNull())
            obj = doc.object();
    }
    file.close();
    //qDebug() << obj;
    return obj;
}
