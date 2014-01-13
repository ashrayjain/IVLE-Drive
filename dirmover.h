#ifndef DIRMOVER_H
#define DIRMOVER_H

#include <QObject>
#include <QDir>

class DirMover : public QObject
{
    Q_OBJECT
public:
    explicit DirMover(QDir _old, QDir _new, QObject *parent = 0);
    void startCopy();
signals:

public slots:

private:
    QDir newDir, oldDir;
    void moveDir(QString dirName);
    void moveFile(QString fileName);
};

#endif // DIRMOVER_H
