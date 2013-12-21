#include "filelistdelegate.h"

#include <QPainter>
#include <QDebug>
#include <QJsonObject>

FileListDelegate::FileListDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void FileListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::SolidLine);

    if (option.state & QStyle::State_Selected) {
        painter->setBrush(option.palette.highlightedText());
        painter->drawText(20, 50*index.row() + 32, ((index.model()->data(index)).toJsonObject())["name"].toString());
        painter->drawText(20, 50*index.row() + 32, ((index.model()->data(index)).toJsonObject())["name"].toString());
    }
    else {
        painter->setBrush(option.palette.text());
        painter->drawText(20, 50*index.row() + 32, ((index.model()->data(index)).toJsonObject())["name"].toString());
    }

    painter->restore();
}

QSize FileListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    return QSize(option.rect.width(), 50);

}
