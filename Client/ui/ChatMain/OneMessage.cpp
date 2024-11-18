// 聊天主窗口
#include "OneMessage.h"
OneMessage::OneMessage(message_info message_info, bool isClient, QWidget *parent)
    : QWidget(parent), m_message_info(message_info)
{
    m_hlayout = new QHBoxLayout(this);
    m_avatar = new Avatar(message_info.sender, this);
    m_messageBubble = new MessageBubble(message_info, isClient, this);
    this->setLayout(m_hlayout);
    if (isClient)//发送方是自己
    {
        m_hlayout->addStretch();
        m_hlayout->addWidget(m_messageBubble);
        m_hlayout->addWidget(m_avatar);
    }
    else
    {
        m_hlayout->addWidget(m_avatar);
        m_hlayout->addWidget(m_messageBubble);
        m_hlayout->addStretch();
    }
    show();
}

OneMessage::~OneMessage()
{
}
