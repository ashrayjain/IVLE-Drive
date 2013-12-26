#include "workbinsviewitem.h"
#include <QDebug>
#include <QApplication>

WorkbinsViewItem::WorkbinsViewItem(WorkbinsViewItem *parent)
{
    parentItem = parent;
    checked = QVariant(Qt::Unchecked);
    files = new FileListModel(QList<QJsonObject>(), this);
    connect(files, SIGNAL(checkBoxChanged()), this, SLOT(childCheckBoxChanged()));
}

WorkbinsViewItem::~WorkbinsViewItem()
{
    qDeleteAll(childItems);
}

void WorkbinsViewItem::appendChild(WorkbinsViewItem *item)
{
    QList<WorkbinsViewItem*>::Iterator it = childItems.begin();
    for (; it != childItems.end() && (*it)->data(0).toString() <= item->data(0).toString(); it++);
    childItems.insert(it, item);

    item->parentItem = this;
    emit childAdded(item);
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

FileListModel *WorkbinsViewItem::fileViewModel()
{
    return files;
}

void WorkbinsViewItem::setFileData(QList<QJsonObject> &data)
{
    disconnect(files, SIGNAL(checkBoxChanged()), this, SLOT(childCheckBoxChanged()));
    delete files;
    files = new FileListModel(data);
    connect(files, SIGNAL(checkBoxChanged()), this, SLOT(childCheckBoxChanged()));
}

void WorkbinsViewItem::appendData(QVariant &data)
{
    itemData.append(data);
}

void WorkbinsViewItem::setChecked(const QVariant& val)
{
    checked = val;

    QList<WorkbinsViewItem*>::ConstIterator it = childItems.constBegin();
    while (it != childItems.constEnd())
        (*it++)->setChecked(val);

    files->setCheckedAll(val);
    handleParents();
    emit childCheckBoxChangedSignal();
}

WorkbinsViewItem *WorkbinsViewItem::parent()
{
    return parentItem;
}

void WorkbinsViewItem::childCheckBoxChanged()
{
    int count = files->checkBoxCount();
    if (count == files->rowCount(QModelIndex()))
        checked = Qt::Checked;
    else if (count == 0)
        checked = Qt::Unchecked;
    else
        checked = Qt::PartiallyChecked;
    handleParents();
    emit childCheckBoxChangedSignal();
}

void WorkbinsViewItem::handleParents()
{
    WorkbinsViewItem* parent = parentItem;
    while (parent != 0) {
        int checkCount = 0;
        int rowCount = parent->childCount();

        bool partialFound = false;
        for (int i = 0; i < rowCount && !partialFound; i++)
            if (parent->child(i)->checked == Qt::Checked)
                checkCount++;
            else if (parent->child(i)->checked == Qt::PartiallyChecked)
                partialFound = true;

        if (partialFound)
            parent->checked = Qt::PartiallyChecked;
        else if (checkCount == 0)
            parent->checked = Qt::Unchecked;
        else if (checkCount == rowCount)
            parent->checked = Qt::Checked;
        else
            parent->checked = Qt::PartiallyChecked;

        parent = parent->parentItem;
    }
}

int WorkbinsViewItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<WorkbinsViewItem*>(this));

    return 0;
}



