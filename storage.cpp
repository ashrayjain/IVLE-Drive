#include "storage.h"
#include <QFile>
#include <QTextStream>
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
