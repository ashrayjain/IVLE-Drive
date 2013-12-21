#include "workbinsviewitem.h"
#include <QDebug>

WorkbinsViewItem::WorkbinsViewItem(const QList<QVariant> &data, WorkbinsViewItem *parent)
{
    parentItem = parent;
    itemData = data;
    checked = QVariant(Qt::Unchecked);
}

WorkbinsViewItem::~WorkbinsViewItem()
{
    qDeleteAll(childItems);
}

void WorkbinsViewItem::appendChild(WorkbinsViewItem *item)
{
    childItems.append(item);
    item->parentItem = this;
}

WorkbinsViewItem *WorkbinsViewItem::child(int row)
{
    return childItems.value(row);
}

int WorkbinsViewItem::childCount() const
{
    return childItems.count();
}

int WorkbinsViewItem::columnCount() const
{
    return itemData.count();
}

QVariant WorkbinsViewItem::data(int column) const
{
    if (column == Qt::CheckStateRole)
        return checked;
    return itemData.value(column);
}

void WorkbinsViewItem::setChecked(const QVariant& val)
{
    checked = val;

    QList<WorkbinsViewItem*>::ConstIterator it = childItems.constBegin();
    while (it != childItems.constEnd())
        (*it++)->setChecked(val);

    WorkbinsViewItem* parent = parentItem;
    while (parent != 0) {
        int checkCount = 0;
        int rowCount = parent->childCount();

        for (int i = 0; i < rowCount; i++)
            if (parent->child(i)->checked == Qt::Checked)
                checkCount++;

        if (checkCount == 0)
            parent->checked = Qt::Unchecked;
        else if (checkCount == rowCount)
            parent->checked = Qt::Checked;
        else
            parent->checked = Qt::PartiallyChecked;

        parent = parent->parentItem;
    }
}

WorkbinsViewItem *WorkbinsViewItem::parent()
{
    return parentItem;
}

int WorkbinsViewItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<WorkbinsViewItem*>(this));

    return 0;
}



