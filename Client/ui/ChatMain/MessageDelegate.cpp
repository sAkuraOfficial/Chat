#include "MessageDelegate.h"
// 静态变量初始化
int MessageDelegate::m_fontSize = 12; // 默认字体大小为12

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
    painter->setBrush(option.state & QStyle::State_Selected ? QColor(245, 245, 245) : QColor(245, 245, 245));
    painter->drawRect(rect);

    // 设置文字字体和大小
    QFont customFont = option.font;      // 获取当前字体
    customFont.setPointSize(m_fontSize); // 使用静态变量管理字体大小
    painter->setFont(customFont);        // 应用字体

    // 动态调整内边距
    int bubblePadding = m_fontSize; // 根据字体大小调整气泡内边距
    int fixedAvatarMargin = 15;     // 固定的头像与气泡之间的距离

    // 头像区域
    int avatarSize = 40;
    QRect avatarRect;

    // 消息气泡区域
    QColor bubbleColor;

    // 动态计算文本宽度和高度
    int textMaxWidth = rect.width() - avatarSize - fixedAvatarMargin * 2; // 可用宽度
    QFontMetrics metrics(customFont);
    QString displayText = message.message;
    QRect textRect = metrics.boundingRect(0, 0, textMaxWidth, 0, Qt::TextWordWrap, displayText);

    QRect bubbleRect;

    if (message.receiver_is_client)
    {
        // 好友发送（气泡在左侧）
        bubbleColor = QColor(137, 217, 97);
        avatarRect = QRect(rect.left() + 5, rect.top() + 5, avatarSize, avatarSize);
        bubbleRect = QRect(
            rect.left() + avatarSize + fixedAvatarMargin, // 气泡起点，头像与气泡之间保持固定距离
            rect.top() + 5,                               // 气泡顶部
            textRect.width() + bubblePadding * 2,         // 气泡宽度
            textRect.height() + bubblePadding * 2         // 气泡高度
        );
    }
    else
    {
        // 客户端发送（气泡在右侧）
        bubbleColor = QColor(235, 235, 235);
        avatarRect = QRect(rect.right() - avatarSize - 5, rect.top() + 5, avatarSize, avatarSize);
        bubbleRect = QRect(
            rect.right() - avatarSize - fixedAvatarMargin - textRect.width() - bubblePadding * 2, // 气泡起点
            rect.top() + 5,                                                                       // 气泡顶部
            textRect.width() + bubblePadding * 2,                                                 // 气泡宽度
            textRect.height() + bubblePadding * 2                                                 // 气泡高度
        );
    }

    // 绘制头像
    painter->drawPixmap(avatarRect, QPixmap(":/Client/profile")); // 替换为实际头像路径

    // 绘制气泡背景
    painter->setBrush(bubbleColor);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(bubbleRect, 10, 10);

    // 显示文本
    QRect adjustedTextRect = bubbleRect.adjusted(bubblePadding, bubblePadding, -bubblePadding, -bubblePadding); // 气泡内边距
    painter->setPen(Qt::black);
    painter->drawText(adjustedTextRect, Qt::TextWordWrap, displayText);

    painter->restore();
}


void MessageDelegate::setFontSize(int fontSize)
{
    m_fontSize = fontSize;
}

int MessageDelegate::getFontSize()
{
    return m_fontSize;
}

QSize MessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 从模型中获取消息信息
    message_info message = index.data(Qt::UserRole).value<message_info>();
    QString displayText = QString("%1: %2").arg(message.sender.username).arg(message.message);

    // 自定义字体
    QFont customFont = option.font;
    customFont.setPointSize(m_fontSize); // 使用静态变量管理字体大小
    QFontMetrics metrics(customFont);

    // 动态调整内边距
    int bubblePadding = m_fontSize; // 根据字体大小调整气泡内边距
    int fixedAvatarMargin = 10;     // 固定的头像与气泡之间的距离

    // 计算可用宽度
    int avatarWidth = 50;                                                         // 头像宽度
    int textMaxWidth = option.rect.width() - avatarWidth - fixedAvatarMargin * 2; // 计算气泡最大宽度

    QRect textRect = metrics.boundingRect(0, 0, textMaxWidth, 0, Qt::TextWordWrap, displayText);
    int textHeight = textRect.height();

    // 头像与气泡之间的间距
    int avatarHeight = 50;
    int bubbleHeight = textHeight + bubblePadding * 2; // 根据字体大小动态调整气泡高度

    int totalHeight = qMax(avatarHeight, bubbleHeight); // 确保气泡高度至少等于头像高度

    return QSize(option.rect.width(), totalHeight); // 返回行的总宽度和动态高度
}
