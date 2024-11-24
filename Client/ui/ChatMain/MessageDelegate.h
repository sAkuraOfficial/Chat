#pragma once
#include <DataTypes/DataTypes.h>
#include <QPainter>
#include <QStyledItemDelegate>

class MessageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
  public:
    explicit MessageDelegate(QObject *parent = nullptr);
    static void setFontSize(int fontSize); // 设置字体大小
    static int getFontSize();              // 获取字体大小
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

  private:
    static int m_fontSize; // 静态变量用于统一管理字体大小
};
