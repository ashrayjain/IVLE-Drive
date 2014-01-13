#ifndef WORKBINDATA_H
#define WORKBINDATA_H

#include <QAbstractListModel>
#include <QObject>
#include <QIcon>
#include <QJsonObject>

class FileListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FileListModel(QList<QJsonObject> &newFileList, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void setCheckedAll(const QVariant& val);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int checkBoxCount() const;
    QJsonObject getFile(int index);

signals:
    void checkBoxChanged();

private:
    QList<QJsonObject> fileList;
    QVector<QIcon> iconsList;

    void processIcons();
    static bool lessThan(const QJsonObject& t1, const QJsonObject& t2);
};

#endif // WORKBINDATA_H
