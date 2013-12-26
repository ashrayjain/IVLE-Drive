#ifndef WORKBINSVIEWITEM_H
#define WORKBINSVIEWITEM_H

#include <QObject>
#include <QList>
#include <QVariant>
#include "filelistmodel.h"

class WorkbinsViewItem : public QObject
{
    Q_OBJECT
public:
    explicit WorkbinsViewItem(WorkbinsViewItem *parent = 0);
    ~WorkbinsViewItem();
    void appendChild(WorkbinsViewItem *child);
    WorkbinsViewItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    FileListModel* fileViewModel();
    void setFileData(QList<QJsonObject> &data);
    void appendData(QVariant& data);
    void setChecked(const QVariant&);
    int row() const;
    WorkbinsViewItem *parent();

 public slots:
    void childCheckBoxChanged();

signals:
    void childAdded(WorkbinsViewItem*);
    void childCheckBoxChangedSignal();

 private:
    QVariant checked;
    QList<WorkbinsViewItem*> childItems;
    QList<QVariant> itemData;
    FileListModel* files;
    WorkbinsViewItem *parentItem;

    void handleParents();
};

#endif // WORKBINSVIEWITEM_H
