#include <QObject>
#include <QtAlgorithms>
#include <QIcon>
#include <QDebug>
#include "storage.h"
#include "filelistmodel.h"

FileListModel::FileListModel(QList<QJsonObject> &newFileList, QObject *parent) :
    QAbstractListModel(parent),
    iconsList(QVector<QIcon>())
{
    beginResetModel();
    fileList = newFileList;
    qSort(fileList.begin(), fileList.end(), lessThan);
    processIcons();
    endResetModel();
}

int FileListModel::rowCount(const QModelIndex &) const {
    return fileList.size();
}

bool FileListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::CheckStateRole) {
        if (value == Qt::Checked)
            fileList[index.row()]["checked"] = true;
        else if (value == Qt::Unchecked)
            fileList[index.row()]["checked"] = false;

        emit checkBoxChanged();
        emit dataChanged(index, index);
        return true;
    }
    else if (index.isValid() && role == Qt::DecorationRole)
        iconsList[index.row()] = value.value<QIcon>();
    return false;
}

void FileListModel::setCheckedAll(const QVariant &val)
{
    bool valToSet = (val == Qt::Checked)?true:false;
    for (QList<QJsonObject>::Iterator it = fileList.begin(); it != fileList.end(); it++)
        (*it)["checked"] = valToSet;
    emit dataChanged(index(0), index(rowCount(QModelIndex())));
}

QVariant FileListModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::UserRole)
        return fileList[index.row()]["id"];
    if (role == Qt::CheckStateRole)
        return fileList[index.row()]["checked"]==true?Qt::Checked:Qt::Unchecked;
    if (role == Qt::DecorationRole)
        return iconsList[index.row()].pixmap(QSize(32, 32));
    if (role != Qt::DisplayRole || !index.isValid())
        return QVariant();
    return fileList[index.row()]["name"];
}

Qt::ItemFlags FileListModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index)|Qt::ItemIsUserCheckable;
}

int FileListModel::checkBoxCount() const
{
    int count = 0;
    for (QList<QJsonObject>::ConstIterator it = fileList.constBegin(); it != fileList.constEnd(); it++)
        if ((*it)["checked"].toBool() == true)
            count++;
    return count;
}

void FileListModel::processIcons()
{
    for (QList<QJsonObject>::ConstIterator it = fileList.constBegin(); it != fileList.constEnd(); it++)
        iconsList.append(Storage::getFileIcon((*it)["type"].toString()));
}

bool FileListModel::lessThan(const QJsonObject& t1, const QJsonObject& t2)
{
    return t1["name"].toString()<t2["name"].toString();
}
