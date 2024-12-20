#include "core.h"
#include "Logger/logger.h"
#include <QEventLoop>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qtimer.h>
#include <qwebsocket.h>
Core::Core(QObject *parent)
    : QObject(parent)
{
    m_pProtocol = new Protocol();

    // 初始化定时器
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(m_pProtocol->getWebSocket(), &QWebSocket::connected, this, [=]() {
        timer->stop();
        emit ConnectSuccess();
    });
    connect(timer, &QTimer::timeout, this, [=]() {
        m_pProtocol->getWebSocket()->close(); // 超时关闭连接
        emit ConnectTimeOut();
    });
    connect(m_pProtocol, &Protocol::ReceiveNewMessage, this, &Core::onReceiveNewMessage); // 接收到新消息，然后路由转发到对应处理函数
}

Core::~Core()
{
}

Protocol *Core::getProtocol()
{
    return m_pProtocol;
}

bool Core::isConnecting()
{
    return m_pProtocol->getWebSocket()->state() == QAbstractSocket::SocketState::ConnectingState;
}

void Core::runClient(QString ws, int timeoutMs)
{
    m_pProtocol->ConnectToServer(ws);
    timer->start(timeoutMs);
    emit beginConnect();
}

void Core::onReceiveNewMessage(QString message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    // Logger::getInstance().log("\n"+ doc.toJson());
    QJsonObject json = doc.object();
    QString type = json["type"].toString();
    if (type == "login")
    {
        processLogin(json);
    }
    else if (type == "register")
    {
        processRegister(json);
    }
    else if (type == "getFriendList")
    {
        processGetFriendList(json);
    }
    else if (type == "newChatMessage")
    {
        processNewMessage(json);
    }
}

//-----------------------------------------------------发送消息-----------------------------------------------------

void Core::login(QString username, QString password)
{
    QJsonObject json;
    json["type"] = "login";
    json["username"] = username;
    json["password"] = password;
    QJsonDocument doc(json);
    m_pProtocol->sendMessage(doc.toJson());
}

void Core::registerUser(QString username, QString password)
{
    QJsonObject json;
    json["type"] = "register";
    json["username"] = username;
    json["password"] = password;
    QJsonDocument doc(json);
    m_pProtocol->sendMessage(doc.toJson());
}

void Core::getFriendList(int user_id)
{
    QJsonObject json;
    json["type"] = "getFriendList";
    json["user_id"] = user_id;
    QJsonDocument doc(json);
    m_pProtocol->sendMessage(doc.toJson());
}

void Core::sendMessage(message_info message)
{
    QJsonObject json;
    json["type"] = "newChatMessage";
    json["sender_id"] = message.sender.user_id;
    json["receiver_id"] = message.receiver.user_id;
    json["content"] = message.message;
    QJsonDocument doc(json);
    m_pProtocol->sendMessage(doc.toJson());
}

void Core::setClientUserInfo(user_info *client_user_info)
{
    m_client_user_info = client_user_info;
}

//-----------------------------------------------------处理接收的消息-----------------------------------------------------

void Core::processLogin(QJsonObject msg_json)
{
    bool result = msg_json["result"].toBool();
    int user_id = result ? msg_json["user_id"].toInt() : -1;
    emit ReceiveLoginResult(result, user_id);
}

void Core::processRegister(QJsonObject msg_json)
{
}

void Core::processGetFriendList(QJsonObject msg_json)
{
    QVector<friend_info> friends;
    QJsonArray friendArray = msg_json["friends"].toArray();
    for (int i = 0; i < friendArray.size(); i++)
    {
        QJsonObject friendObj = friendArray.at(i).toObject();
        friend_info temp_friend;
        temp_friend.user_id = friendObj["user_id"].toInt();
        temp_friend.username = friendObj["username"].toString();
        temp_friend.status = friend_status_code::GetStatusCode(friendObj["status"].toString());
        temp_friend.isOnline = false;
        temp_friend.last_message.message = "好饿呀";
        temp_friend.last_message.time = QDateTime::currentDateTime();
        friends.push_back(temp_friend);
    }
    emit ReceiveGetFriendList(friends);
}

void Core::processNewMessage(QJsonObject msg_json)
{
    Logger::getInstance().log("收到新消息" + QJsonDocument(msg_json).toJson());
    message_info msg;
    msg.sender.user_id = msg_json["sender_id"].toInt();
    msg.receiver.user_id = msg_json["receiver_id"].toInt();
    msg.message = msg_json["content"].toString();
    msg.time = QDateTime::currentDateTime();
    msg.receiver_is_client = m_client_user_info->user_id == msg.receiver.user_id;
    emit ReceiveUserMessage(msg);
}
