// 聊天主窗口
#include "MessageBubble.h"
MessageBubble::MessageBubble(message_info message_info, bool isClient, QWidget *parent)
    : QLabel(parent), m_message_info(message_info)
{
    setMaximumWidth(400);
    setStyleSheet("background-color:blue;");
    setText(message_info.message);


}

MessageBubble::~MessageBubble()
{
}
