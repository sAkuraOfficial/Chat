// 聊天主窗口
#include "MessageBubble.h"
MessageBubble::MessageBubble(message_info message_info, bool isClient, QWidget *parent)
    : QLabel(parent), m_message_info(message_info)
{
    setMaximumWidth(400);
    setText(message_info.message);
    if (isClient)
    {
        setStyleSheet("background-color:rgb(137, 217, 97);border-radius:25px;");
    }
    else
    {
        setStyleSheet("background-color:rgb(235, 235, 235);border-radius:25px;");
    }
    show();
}

MessageBubble::~MessageBubble()
{
}
