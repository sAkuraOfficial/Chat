// 聊天主窗口
#include "ClientMain.h"
#include "../ClientLogin/ClientLogin.h"
#include "FriendWidget.h"
#include <QTimer>
ClientMain::ClientMain(Core *core, QWidget *parent)
    : QMainWindow(parent), m_core(core)
{
    ui.setupUi(this);
}

ClientMain::~ClientMain()
{
}

void ClientMain::onLoginSuccess(QString username, int user_id)
{
    // 连接slot
    connect(m_core, &Core::ReceiveGetFriendList, this, &ClientMain::onReceiveGetFriendList);

    m_username = username;
    this->show();
    this->setWindowTitle("欢迎您，" + m_username);

    // 获取好友列表
    m_core->getFriendList(user_id);
}

void ClientMain::onReceiveGetFriendList(QVector<friend_info> friends)
{
    m_friends = friends;
    ui.listWidget_friends->clear();
    for (auto &temp_friend_info : m_friends)
    {
        FriendWidget *item_widget = new FriendWidget(temp_friend_info, this);
        item_widget->setFixedSize(250, 100);
        QListWidgetItem *item = new QListWidgetItem(ui.listWidget_friends);
        item->setSizeHint(QSize(250, 75));
        ui.listWidget_friends->addItem(item);
        ui.listWidget_friends->setItemWidget(item, item_widget);
    }

}