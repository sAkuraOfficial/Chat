// 聊天主窗口
#include "FriendWidget.h"
FriendWidget::FriendWidget(friend_info friend_info, QWidget *parent)
    : QMainWindow(parent), m_friend_info(friend_info)
{
    ui.setupUi(this);
    ui.label_username->setText(m_friend_info.username);
    ui.label_content->setText(m_friend_info.last_message.message);
    ui.label_time->setText(m_friend_info.last_message.time.toString("HH:mm"));

}

FriendWidget::~FriendWidget()
{
}
