#ifndef WORKBINSVIEWMODEL_H
#define WORKBINSVIEWMODEL_H

#include <QAbstractItemModel>
#include "workbinsviewitem.h"

class WorkbinsViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit WorkbinsViewModel(QObject *parent = 0);
    ~WorkbinsViewModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    WorkbinsViewItem* invisibleRootItem();
    QModelIndex& rootIndex();

public slots:
    void childAddedSlot(WorkbinsViewItem*);
    void childCheckBoxChanged();

private:
    WorkbinsViewItem *rootItem;
};

#endif // WORKBINSVIEWMODEL_H
