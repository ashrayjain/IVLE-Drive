#include <QObject>
#include "filelistmodel.h"

FileListModel::FileListModel(QVector<QJsonObject> &newFileList, QObject *parent) :
    QAbstractListModel(parent)
{
    beginResetModel();
    fileList = newFileList;
    endResetModel();
}

int FileListModel::rowCount(const QModelIndex &) const {
    return fileList.size();
}

QVariant FileListModel::data(const QModelIndex &index, int role) const {
    if (role != Qt::DisplayRole || !index.isValid())
        return QVariant();
    return fileList[index.row()];
}

