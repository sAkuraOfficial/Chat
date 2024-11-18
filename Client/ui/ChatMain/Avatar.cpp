// 聊天主窗口
#include "Avatar.h"
Avatar::Avatar(user_info user_info, QWidget *parent)
    : QWidget(parent), m_user_info(user_info)
{
    QByteArray ba = user_info.avatar;
    QPixmap pixmap;
    pixmap.loadFromData(ba);
    m_avatar->setPixmap(pixmap);
    m_avatar->setGeometry(0, 0, 54, 54);
    m_avatar->show();
}

Avatar::~Avatar()
{
}
