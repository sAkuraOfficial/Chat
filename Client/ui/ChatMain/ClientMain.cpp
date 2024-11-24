// 聊天主窗口
#include "ClientMain.h"
#include "../ClientLogin/ClientLogin.h"
#include "FriendWidget.h"
#include <Logger/logger.h>
#include <QTimer>
ClientMain::ClientMain(Core *core, QWidget *parent)
    : QMainWindow(parent), m_core(core)
{
    ui.setupUi(this);
    // 连接slot
    connect(m_core, &Core::ReceiveUserMessage, this, &ClientMain::onReceiveUserMessage);
}

ClientMain::~ClientMain()
{
}

user_info ClientMain::GetClientUserInfo()
{
    return m_client_user_info;
}

void ClientMain::onLoginSuccess(QString username, int user_id)
{
    // 连接slot
    connect(m_core, &Core::ReceiveGetFriendList, this, &ClientMain::onReceiveGetFriendList);

    m_client_user_info.username = username;
    m_client_user_info.user_id = user_id;
    m_core->setClientUserInfo(&m_client_user_info);
    this->show();
    this->setWindowTitle("欢迎您，" + m_client_user_info.username);

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
        item->setData(Qt::UserRole, temp_friend_info.user_id); // 设置元对象friend_id
        ui.listWidget_friends->addItem(item);
        ui.listWidget_friends->setItemWidget(item, item_widget);
    }
}

void ClientMain::onSendMessageToServer(message_info message)
{
    m_core->sendMessage(message);
}

void ClientMain::on_listWidget_friends_itemClicked(QListWidgetItem *item)
{
    // 从元对象 friend_id 获取到 friend_id
    int friend_id = item->data(Qt::UserRole).toInt();
    Logger::getInstance().log("点击了好友，friend_id: " + QString::number(friend_id));

    // 检查是否已经存在该好友的聊天窗口
    if (m_chatWidgets.contains(friend_id))
    {
        // 如果存在，切换到对应的聊天窗口
        ChatWidget *chatWidget = m_chatWidgets[friend_id];
        int index = ui.stackedWidget->indexOf(chatWidget);
        if (index != -1)
        {
            ui.stackedWidget->setCurrentIndex(index);
            Logger::getInstance().log("切换到现有聊天窗口，friend_id: " + QString::number(friend_id));
        }
    }
    else
    {
        // 如果没有，创建一个新的聊天窗口
        friend_info friendInfo;
        for (const auto &friendItem : m_friends)
        {
            if (friendItem.user_id == friend_id)
            {
                friendInfo = friendItem;
                break;
            }
        }

        // 创建新的 ChatWidget
        ChatWidget *newChatWidget = new ChatWidget(friendInfo, m_client_user_info, this);
        connect(newChatWidget, &ChatWidget::SendMessageToServer, this, &ClientMain::onSendMessageToServer);

        // 添加到 QStackedWidget 中
        int newIndex = ui.stackedWidget->addWidget(newChatWidget);
        ui.stackedWidget->setCurrentIndex(newIndex);

        // 将新创建的 ChatWidget 存储在 map 中
        m_chatWidgets.insert(friend_id, newChatWidget);

        Logger::getInstance().log("创建新的聊天窗口，friend_id: " + QString::number(friend_id));
    }
}

void ClientMain::onReceiveUserMessage(message_info message)
{
    int sender_id = message.sender.user_id;

    // 检查是否已有该用户的聊天小部件
    if (m_chatWidgets.contains(sender_id))
    {
        // 如果存在，切换到对应的聊天窗口
        ChatWidget *chatWidget = m_chatWidgets[sender_id];
        int index = ui.stackedWidget->indexOf(chatWidget);
        if (index != -1)
        {
            ui.stackedWidget->setCurrentIndex(index);
        }
        // 添加消息到已有的聊天窗口
        chatWidget->addMessage(message);
    }
    else
    {
        // 如果没有，创建一个新的聊天窗口并添加到stackedWidget
        friend_info friendInfo;
        for (const auto &friendItem : m_friends)
        {
            if (friendItem.user_id == sender_id)
            {
                friendInfo = friendItem;
                break;
            }
        }

        // 创建新的 ChatMessageList
        ChatWidget *newChatWidget = new ChatWidget(friendInfo, m_client_user_info, this);
        connect(newChatWidget, &ChatWidget::SendMessageToServer, this, &ClientMain::onSendMessageToServer);
        newChatWidget->addMessage(message);

        // 添加到 QStackedWidget 中
        int newIndex = ui.stackedWidget->addWidget(newChatWidget);
        ui.stackedWidget->setCurrentIndex(newIndex);

        // 将新创建的 ChatMessageList 存储在 map 中以备将来使用
        m_chatWidgets.insert(sender_id, newChatWidget);
    }
}
