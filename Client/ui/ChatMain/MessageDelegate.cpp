#include "MessageDelegate.h"

MessageDelegate::MessageDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void MessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 从模型中获取消息信息
    message_info message = index.data(Qt::UserRole).value<message_info>();
    QRect rect = option.rect;

    // 绘制背景
    painter->setPen(Qt::NoPen);
    painter->setBrush(option.state & QStyle::State_Selected ? QColor(235, 235, 235) : QColor(245, 245, 245));
    painter->drawRect(rect);

    // 头像区域
    int avatarSize = 40;
    QRect avatarRect;

    // 消息气泡区域
    int bubbleMargin = 15;
    QColor bubbleColor;

    // 动态计算文本宽度和高度
    int textMaxWidth = rect.width() - avatarSize - bubbleMargin * 2; // 可用宽度
    QFontMetrics metrics(option.font);
    QString displayText = message.message;
    QRect textRect = metrics.boundingRect(0, 0, textMaxWidth, 0, Qt::TextWordWrap, displayText);

    QRect bubbleRect;

    if (message.receiver_is_client)
    {
        // 好友发送（气泡在左侧）
        bubbleColor = QColor(137, 217, 97);
        avatarRect = QRect(rect.left() + 5, rect.top() + 5, avatarSize, avatarSize);
        bubbleRect = QRect(
            rect.left() + avatarSize + bubbleMargin, // 气泡起点
            rect.top() + 5,                          // 气泡顶部
            textRect.width() + 20,                   // 气泡宽度
            textRect.height() + 20                   // 气泡高度
        );
    }
    else
    {
        // 客户端发送（气泡在右侧）
        bubbleColor = QColor(235, 235, 235);
        avatarRect = QRect(rect.right() - avatarSize - 5, rect.top() + 5, avatarSize, avatarSize);
        bubbleRect = QRect(
            rect.right() - avatarSize - bubbleMargin - textRect.width() - 20, // 气泡起点
            rect.top() + 5,                                                   // 气泡顶部
            textRect.width() + 20,                                            // 气泡宽度
            textRect.height() + 20                                            // 气泡高度
        );
    }

    // 绘制头像
    painter->drawPixmap(avatarRect, QPixmap(":/Client/profile")); // 替换为实际头像路径

    // 绘制气泡背景
    painter->setBrush(bubbleColor);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(bubbleRect, 10, 10);

    // 显示文本
    QRect adjustedTextRect = bubbleRect.adjusted(10, 10, -10, -10); // 气泡内边距
    painter->setPen(Qt::black);
    painter->drawText(adjustedTextRect, Qt::TextWordWrap, displayText);

    painter->restore();
}


QSize MessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 从模型中获取消息信息
    message_info message = index.data(Qt::UserRole).value<message_info>();
    QString displayText = QString("%1: %2").arg(message.sender.username).arg(message.message);

    // 计算可用宽度
    int avatarWidth = 50;                                              // 头像宽度
    int margin = 20;                                                   // 气泡左右边距
    int textMaxWidth = option.rect.width() - avatarWidth - margin * 2; // 计算气泡最大宽度

    // 使用 QFontMetrics 计算文本高度
    QFontMetrics metrics(option.font);
    QRect textRect = metrics.boundingRect(0, 0, textMaxWidth, 0, Qt::TextWordWrap, displayText);
    int textHeight = textRect.height();

    // 头像与气泡之间的间距
    int avatarHeight = 50;
    int padding = 20; // 顶部和底部额外的间距
    int bubbleHeight = qMax(avatarHeight, textHeight + padding);

    return QSize(option.rect.width(), bubbleHeight); // 返回行的总宽度和动态高度
}
