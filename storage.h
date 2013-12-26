#ifndef STORAGE_H
#define STORAGE_H

class QString;
class QIcon;

class Storage
{
public:
    static QString readToken();
    static void writeToken(QString &token);
    static QIcon getFileIcon(QString ext);
};

#endif // STORAGE_H
