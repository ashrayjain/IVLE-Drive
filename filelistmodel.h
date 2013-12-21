#ifndef WORKBINDATA_H
#define WORKBINDATA_H

#include <QAbstractListModel>
#include <QObject>
#include <QJsonObject>

class FileListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FileListModel(QVector<QJsonObject> &newFileList, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QVector<QJsonObject> fileList;

};

#endif // WORKBINDATA_H
