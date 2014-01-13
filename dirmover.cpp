#include "dirmover.h"
#include <QDebug>

DirMover::DirMover(QDir _old, QDir _new, QObject *parent) :
    QObject(parent)
{
    oldDir = _old;
    newDir = _new;
}

void DirMover::startCopy()
{
    newDir.mkdir("IVLEDrive");
    newDir.cd("IVLEDrive");
    oldDir.cd("IVLEDrive");

    foreach(QString entry, oldDir.entryList(QDir::NoDotAndDotDot|QDir::Dirs))
        moveDir(entry);

    // REMOVE
    oldDir.cdUp();
    newDir.cdUp();
    oldDir.rmdir("IVLEDrive");
}

void DirMover::moveDir(QString dirName)
{
    newDir.mkdir(dirName);
    newDir.cd(dirName);
    oldDir.cd(dirName);
    foreach(QString entry, oldDir.entryList(QDir::NoDotAndDotDot|QDir::Dirs))
        moveDir(entry);
    foreach(QString entry, oldDir.entryList(QDir::NoDotAndDotDot|QDir::Files))
        moveFile(entry);
    oldDir.cdUp();
    newDir.cdUp();
    oldDir.rmdir(dirName);
}

void DirMover::moveFile(QString fileName)
{
    QFile::copy(oldDir.path()+"/"+fileName, newDir.path()+"/"+fileName);
    QFile::remove(oldDir.path()+"/"+fileName);
}
