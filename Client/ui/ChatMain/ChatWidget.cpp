// 聊天主窗口
#include "ChatWidget.h"
ChatWidget::ChatWidget(friend_info friend_info, QWidget *parent)
    : QWidget(parent), m_friend_info(friend_info)
{
    m_vlayout = new QVBoxLayout(this);
    this->setLayout(m_vlayout);
    m_vlayout->addWidget(m_list);

    m_data_model = new QStandardItemModel();
    m_list->setModel(m_data_model);

}

ChatWidget::~ChatWidget()
{
}

void ChatWidget::AddMessage(message_info message)
{
    user_info sender = message.sender;
    user_info receiver = message.receiver;
    if (sender.username == m_friend_info.username)
    {
        // 对方发来的消息
        OneMessage *oneMessage = new OneMessage(message, false, this);
    }
    else if (receiver.username == m_friend_info.username)
    {
        // 自己发出的消息
        OneMessage *oneMessage = new OneMessage(message, true, this);
    }
}
