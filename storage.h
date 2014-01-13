#ifndef STORAGE_H
#define STORAGE_H

class QString;
class QIcon;
class QJsonObject;

class Storage
{
public:
    static QString readToken();
    static void writeToken(QString &token);
    static QIcon getFileIcon(QString ext);
    static QString getDownloadDir();
    static void writeDownloadDir(QString &dir);
    static void saveState(QJsonObject &state);
    static QJsonObject loadState();
};

#endif // STORAGE_H
