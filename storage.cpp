#include "storage.h"
#include <QFile>
#include <QTextStream>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QFileIconProvider>
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
