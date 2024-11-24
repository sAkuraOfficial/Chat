// 聊天主窗口
#include "ChatWidget.h"
ChatWidget::ChatWidget(friend_info friend_info, user_info client_user_info,QWidget *parent)
    : QWidget(parent), m_friend_info(friend_info), m_client_user_info(client_user_info)
{
    ui.setupUi(this);
    ui.label_sessionName->setText(m_friend_info.username);
    m_messageList = new ChatMessageList(m_friend_info, this);
    QVBoxLayout *layout = new QVBoxLayout(ui.widget);
    layout->setContentsMargins(0, 0, 0, 0); // 设置边距为 0，确保子控件充满整个 widget
    layout->setSpacing(0);                  // 设置控件之间的间距为 0

    // 将 m_messageList 添加到布局中
    layout->addWidget(m_messageList);

    // 设置布局为 ui.widget 的布局
    ui.widget->setLayout(layout);
}

ChatWidget::~ChatWidget()
{
}

void ChatWidget::addMessage(message_info message)
{
    m_messageList->AddMessage(message);
}

void ChatWidget::setSessionName(QString session_name)
{
    ui.label_sessionName->setText(session_name);
}

void ChatWidget::on_pushButton_sendMessage_clicked()
{
    message_info message;
    message.receiver = m_friend_info;
    message.sender = m_client_user_info;
    message.message = ui.textEdit_message->toPlainText();
    message.receiver_is_client = false;//接收方不是本机客户端
    message.time = QDateTime::currentDateTime();
    m_messageList->AddMessage(message);
    emit SendMessageToServer(message);
} 