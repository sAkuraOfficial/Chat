#pragma once
#include <DataTypes/DataTypes.h>
#include <QPainter>
#include <QStyledItemDelegate>

class MessageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
  public:
    explicit MessageDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
