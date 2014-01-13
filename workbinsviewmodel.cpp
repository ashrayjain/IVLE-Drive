#include "workbinsviewmodel.h"
#include "workbinsviewitem.h"

#include <QStringList>
#include <QDebug>
#include <QStandardItemModel>

WorkbinsViewModel::WorkbinsViewModel(QObject *parent)
    : QAbstractItemModel(parent),
      rootItem(new WorkbinsViewItem())
{
    connect(rootItem, SIGNAL(childCheckBoxChangedSignal()), this, SLOT(childCheckBoxChanged()));
    connect(rootItem, SIGNAL(childAdded(WorkbinsViewItem*)), this, SLOT(childAddedSlot(WorkbinsViewItem*)));
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
        WorkbinsViewItem* item = static_cast<WorkbinsViewItem*>(index.internalPointer());
        item->setChecked(value);
        emit dataChanged(QModelIndex(), createIndex(item->parent()->childCount()-1, 0, item->parent()));
        return true;
    }
    return false;
}

WorkbinsViewItem *WorkbinsViewModel::invisibleRootItem()
{
    return rootItem;
}

QModelIndex &WorkbinsViewModel::rootIndex()
{
    return this->createIndex(0, 0, rootItem).child(0, 0);
}

void WorkbinsViewModel::childAddedSlot(WorkbinsViewItem* newlyAddedNode)
{
    connect(newlyAddedNode, SIGNAL(childAdded(WorkbinsViewItem*)), this, SLOT(childAddedSlot(WorkbinsViewItem*)));
    connect(newlyAddedNode, SIGNAL(childCheckBoxChangedSignal()),this, SLOT(childCheckBoxChanged()));
}

void WorkbinsViewModel::childCheckBoxChanged()
{
    WorkbinsViewItem* item = static_cast<WorkbinsViewItem*>(sender());
    emit dataChanged(QModelIndex(), createIndex(item->parent()->childCount()-1, 0, item->parent()));
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
        return item->data(0);
    case Qt::ToolTipRole:
        return item->data(1);
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


