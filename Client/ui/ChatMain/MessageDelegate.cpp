#include "MessageDelegate.h"

MessageDelegate::MessageDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void MessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // ��ģ���л�ȡ��Ϣ��Ϣ
    message_info message = index.data(Qt::UserRole).value<message_info>();
    QRect rect = option.rect;

    // ���Ʊ���
    painter->setPen(Qt::NoPen);
    painter->setBrush(option.state & QStyle::State_Selected ? QColor(235, 235, 235) : QColor(245, 245, 245));
    painter->drawRect(rect);

    // ͷ������
    int avatarSize = 40;
    QRect avatarRect;

    // ��Ϣ��������
    int bubbleMargin = 15;
    QColor bubbleColor;

    // ��̬�����ı���Ⱥ͸߶�
    int textMaxWidth = rect.width() - avatarSize - bubbleMargin * 2; // ���ÿ��
    QFontMetrics metrics(option.font);
    QString displayText = message.message;
    QRect textRect = metrics.boundingRect(0, 0, textMaxWidth, 0, Qt::TextWordWrap, displayText);

    QRect bubbleRect;

    if (message.receiver_is_client)
    {
        // ���ѷ��ͣ���������ࣩ
        bubbleColor = QColor(137, 217, 97);
        avatarRect = QRect(rect.left() + 5, rect.top() + 5, avatarSize, avatarSize);
        bubbleRect = QRect(
            rect.left() + avatarSize + bubbleMargin, // �������
            rect.top() + 5,                          // ���ݶ���
            textRect.width() + 20,                   // ���ݿ��
            textRect.height() + 20                   // ���ݸ߶�
        );
    }
    else
    {
        // �ͻ��˷��ͣ��������Ҳࣩ
        bubbleColor = QColor(235, 235, 235);
        avatarRect = QRect(rect.right() - avatarSize - 5, rect.top() + 5, avatarSize, avatarSize);
        bubbleRect = QRect(
            rect.right() - avatarSize - bubbleMargin - textRect.width() - 20, // �������
            rect.top() + 5,                                                   // ���ݶ���
            textRect.width() + 20,                                            // ���ݿ��
            textRect.height() + 20                                            // ���ݸ߶�
        );
    }

    // ����ͷ��
    painter->drawPixmap(avatarRect, QPixmap(":/Client/profile")); // �滻Ϊʵ��ͷ��·��

    // �������ݱ���
    painter->setBrush(bubbleColor);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(bubbleRect, 10, 10);

    // ��ʾ�ı�
    QRect adjustedTextRect = bubbleRect.adjusted(10, 10, -10, -10); // �����ڱ߾�
    painter->setPen(Qt::black);
    painter->drawText(adjustedTextRect, Qt::TextWordWrap, displayText);

    painter->restore();
}


QSize MessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // ��ģ���л�ȡ��Ϣ��Ϣ
    message_info message = index.data(Qt::UserRole).value<message_info>();
    QString displayText = QString("%1: %2").arg(message.sender.username).arg(message.message);

    // ������ÿ��
    int avatarWidth = 50;                                              // ͷ����
    int margin = 20;                                                   // �������ұ߾�
    int textMaxWidth = option.rect.width() - avatarWidth - margin * 2; // �������������

    // ʹ�� QFontMetrics �����ı��߶�
    QFontMetrics metrics(option.font);
    QRect textRect = metrics.boundingRect(0, 0, textMaxWidth, 0, Qt::TextWordWrap, displayText);
    int textHeight = textRect.height();

    // ͷ��������֮��ļ��
    int avatarHeight = 50;
    int padding = 20; // �����͵ײ�����ļ��
    int bubbleHeight = qMax(avatarHeight, textHeight + padding);

    return QSize(option.rect.width(), bubbleHeight); // �����е��ܿ�ȺͶ�̬�߶�
}
