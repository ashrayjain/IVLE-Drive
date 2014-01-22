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
    foreach (QJsonObject val, fileList)
        val["checked"] = valToSet;
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
    foreach (QJsonObject val, fileList)
        if (val["checked"].toBool() == true)
            count++;
    return count;
}

QJsonObject FileListModel::getFile(int index)
{
    if (index >=0 && index < rowCount())
        return fileList[index];
    return QJsonObject();
}

void FileListModel::processIcons()
{
    foreach (QJsonObject val, fileList)
        iconsList.append(Storage::getFileIcon(val["type"].toString()));
}

bool FileListModel::lessThan(const QJsonObject& t1, const QJsonObject& t2)
{
    return t1["name"].toString()<t2["name"].toString();
}
