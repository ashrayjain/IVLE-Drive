#ifndef STORAGE_H
#define STORAGE_H

class QString;

class Storage
{
public:
    static QString readToken();
    static void writeToken(QString &token);
};

#endif // STORAGE_H
