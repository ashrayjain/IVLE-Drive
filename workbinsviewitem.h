#ifndef WORKBINSVIEWITEM_H
#define WORKBINSVIEWITEM_H

#include <QList>
#include <QVariant>

class WorkbinsViewItem
{
public:
    explicit WorkbinsViewItem(const QList<QVariant> &data, WorkbinsViewItem *parent = 0);
    ~WorkbinsViewItem();
    void appendChild(WorkbinsViewItem *child);
    WorkbinsViewItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setChecked(const QVariant&);
    int row() const;
    WorkbinsViewItem *parent();

 private:
    QVariant checked;
    QList<WorkbinsViewItem*> childItems;
    QList<QVariant> itemData;
    WorkbinsViewItem *parentItem;

};

#endif // WORKBINSVIEWITEM_H
