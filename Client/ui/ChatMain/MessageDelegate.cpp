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

    // ���Ʊ���
    painter->setPen(Qt::NoPen);
    painter->setBrush(option.state & QStyle::State_Selected ? QColor(220, 220, 220) : QColor(240, 240, 240));
    painter->drawRect(rect);

    // ͷ������
    int avatarSize = 40;
    QRect avatarRect(rect.left() + 5, rect.top() + 5, avatarSize, avatarSize);
    painter->drawPixmap(avatarRect, QPixmap(":/path/to/avatar.png")); // ʹ��ʵ��ͷ��·���� QPixmap ����

    // ��Ϣ��������
    QRect bubbleRect = rect.adjusted(avatarSize + 15, 5, -10, -5);
    painter->setBrush(QColor(200, 230, 255)); // ���ݱ���ɫ
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(bubbleRect, 10, 10); // Բ�Ǿ�������

    // ��ʾ���ı�
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
