#ifndef FILELISTDELEGATE_H
#define FILELISTDELEGATE_H

#include <QAbstractItemDelegate>
#include <QModelIndex>
#include <QSize>

class QObject;
class QPainter;

class FileListDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit FileListDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // FILELISTDELEGATE_H
