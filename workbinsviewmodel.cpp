#include "workbinsviewmodel.h"
#include "workbinsviewitem.h"

#include <QStringList>
#include <QDebug>
#include <QStandardItemModel>

WorkbinsViewModel::WorkbinsViewModel(QObject *parent)
    : QAbstractItemModel(parent),
      rootItem(new WorkbinsViewItem(QList<QVariant>()))
{
}

WorkbinsViewModel::~WorkbinsViewModel()
{
    delete rootItem;
}

int WorkbinsViewModel::columnCount(const QModelIndex &) const
{
    return 1;
}

bool WorkbinsViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::CheckStateRole) {
        static_cast<WorkbinsViewItem*>(index.internalPointer())->setChecked(value);
        QModelIndex curr = index;
        while (curr.isValid())
            curr = curr.parent();
        emit dataChanged(curr, curr);
        return true;
    }
    return false;
}

WorkbinsViewItem *WorkbinsViewModel::invisibleRootItem()
{
    return rootItem;
}

QVariant WorkbinsViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    WorkbinsViewItem *item = static_cast<WorkbinsViewItem*>(index.internalPointer());
    switch(role) {
    case Qt::CheckStateRole:
        return item->data(role);
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return item->data(index.column());
    default:
        return QVariant();
    }
}

Qt::ItemFlags WorkbinsViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index)|Qt::ItemIsUserCheckable;
}

//QVariant WorkbinsViewModel::headerData(int section, Qt::Orientation orientation,
//                               int role) const
//{
//    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
//        return rootItem->data(section);

//    return QVariant();
//}

QModelIndex WorkbinsViewModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    WorkbinsViewItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<WorkbinsViewItem*>(parent.internalPointer());

    WorkbinsViewItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex WorkbinsViewModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    WorkbinsViewItem *childItem = static_cast<WorkbinsViewItem*>(index.internalPointer());
    WorkbinsViewItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int WorkbinsViewModel::rowCount(const QModelIndex &parent) const
{
    WorkbinsViewItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<WorkbinsViewItem*>(parent.internalPointer());

    return parentItem->childCount();
}


