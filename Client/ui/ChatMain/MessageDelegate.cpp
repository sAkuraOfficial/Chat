#include "MessageDelegate.h"

MessageDelegate::MessageDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void MessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    message_info message = index.data(Qt::UserRole).value<message_info>();
    QRect rect = option.rect;

    // 绘制背景
    painter->setPen(Qt::NoPen);
    painter->setBrush(option.state & QStyle::State_Selected ? QColor(220, 220, 220) : QColor(240, 240, 240));
    painter->drawRect(rect);

    // 头像区域
    int avatarSize = 40;
    QRect avatarRect(rect.left() + 5, rect.top() + 5, avatarSize, avatarSize);
    painter->drawPixmap(avatarRect, QPixmap(":/path/to/avatar.png")); // 使用实际头像路径或 QPixmap 对象

    // 消息气泡区域
    QRect bubbleRect = rect.adjusted(avatarSize + 15, 5, -10, -5);
    painter->setBrush(QColor(200, 230, 255)); // 气泡背景色
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(bubbleRect, 10, 10); // 圆角矩形气泡

    // 显示的文本
    QString displayText = QString("%1: %2").arg(message.sender.username).arg(message.message);
    painter->setPen(Qt::black);
    painter->drawText(bubbleRect.adjusted(5, 5, -5, -5), Qt::TextWordWrap, displayText);

    painter->restore();
}

QSize MessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    return QSize(200, 50); // Adjust as needed
}
