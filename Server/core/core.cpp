#include "core.h"
#include <DataTypes/DataTypes.h>
#include <QJsonArray>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
/*
---------------------------编码规范---------------------------
数据库好友列表中，USER1_ID < USER2_ID
调用操作好友列表的时候，必须先确保USER1_ID < USER2_ID
--------------------------------------------------------------
*/

Core::Core(QObject *parent)
    : QObject(parent)
{
    m_pDatabase = new Database(this);
    m_pProtocol = new Protocol(this);
}
Core::~Core()
{
    delete m_pDatabase;
    delete m_pProtocol;
}

bool Core::runServer(int port)
{
    bool isProtocolListening = m_pProtocol->listen(port);
    if (!isProtocolListening)
    {
        Logger::getInstance().log("Protocol监听失败");
        return false;
    }
    else
    {
        Logger::getInstance().log(QString("Protocol监听成功，端口号：%1").arg(port));
    }
    bool isDatabaseOpen = m_pDatabase->openDatabase();
    if (!isDatabaseOpen)
    {
        Logger::getInstance().log("Database连接失败");
        Logger::getInstance().log(m_pDatabase->GetDB().lastError().text(), Logger::ERROR);
        return false;
    }
    else
    {
        Logger::getInstance().log("Database连接成功");
    }
    connect(
        m_pProtocol, &Protocol::newClientMessage,
        this, &Core::onNewMessage
    );
    return true;
}

QVector<client_info> Core::getClientList()
{
    return m_client;
}

void Core::processLogin(QString msg, QWebSocket *sender)
{
    QString username;
    QString password;
    QJsonDocument msg_doc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject msg_obj = msg_doc.object();
    username = msg_obj["username"].toString();
    password = msg_obj["password"].toString();
    QString log = "[登录请求]:用户名:%1\t密码:%2";
    log = log.arg(username).arg(password);
    Logger::getInstance().log(log);
    QSqlQuery query;
    query.prepare("SELECT * FROM USERS WHERE NAME = :username AND PWD = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.exec();
    if (query.lastError().isValid())
    {
        Logger::getInstance().log("登录失败");
    }
    else
    {
        // 从sql查询结果中获取字段ID

        QJsonObject obj;
        obj["type"] = "login";
        if (query.next())
        {
            Logger::getInstance().log("登录成功");
            int user_id;
            user_id = query.value("ID").toInt();
            obj["result"] = true;
            obj["user_id"] = user_id; // 返回用户ID（数据库用户表主键）
            client_info temp_client;
            temp_client.user_id = user_id;
            temp_client.username = username;
            temp_client.isOnline = true;
            temp_client.socket = sender;
            m_client.push_back(temp_client);
            emit clientLogin(temp_client);
        }
        else
        {
            Logger::getInstance().log("登录失败");
            obj["result"] = false;
        }
        QJsonDocument doc(obj);
        QString str(doc.toJson(QJsonDocument::Compact));
        m_pProtocol->sendToClient(str, sender);
    }
}

void Core::processRegister(QString msg, QWebSocket *sender)
{
    QString username;
    QString password;
    QJsonDocument msg_doc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject msg_obj = msg_doc.object();
    username = msg_obj["username"].toString();
    password = msg_obj["password"].toString();
    QString log = "[注册请求]:用户名:%1\t密码:%2";
    log = log.arg(username).arg(password);
    Logger::getInstance().log(log);
    QSqlQuery query;
    query.prepare("INSERT INTO USERS (NAME, PWD) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.exec();
    QJsonObject obj;
    obj["type"] = "register";
    if (query.lastError().isValid())
    {
        Logger::getInstance().log("注册失败");
        obj["result"] = false;
    }
    else
    {
        Logger::getInstance().log("注册成功");
        obj["result"] = true;
    }
    QJsonDocument doc(obj);
    QString str(doc.toJson(QJsonDocument::Compact));
    m_pProtocol->sendToClient(str, sender);
}

void Core::processGetFriendList(QString msg, QWebSocket *sender)
{
    QJsonDocument msg_doc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject msg_obj = msg_doc.object();
    int user_id = msg_obj["user_id"].toInt();
    // 要分两种情况，一种是user_id是USER1_ID，一种是user_id是USER2_ID
    QString log = "[获取好友列表请求]:用户ID:%1";
    log = log.arg(user_id);
    Logger::getInstance().log(log);
    QSqlQuery query;
    query.prepare(R"(
        SELECT
	        USERS.ID,
	        USERS.NAME,
	        USERS.STATUS,
	        FRIENDS.STATUS AS FRIEND_STATUS
        FROM
	        USERS
        JOIN
	        FRIENDS ON (USERS.ID=FRIENDS.USER1_ID AND FRIENDS.USER2_ID=:user1_id)
			        OR (USERS.ID=FRIENDS.USER2_ID AND FRIENDS.USER1_ID=:user2_id)
    )");
    query.bindValue(":user1_id", user_id);
    query.bindValue(":user2_id", user_id);
    query.exec();
    QJsonObject obj;
    obj["type"] = "getFriendList";
    if (query.lastError().isValid())
    {
        Logger::getInstance().log("获取好友列表失败");
        obj["result"] = false;
    }
    else
    {
        Logger::getInstance().log("获取好友列表成功");
        obj["result"] = true;
        QVector<friend_info> friends;
        int i = 0;
        while (query.next())
        {
            friend_info temp_friend;
            temp_friend.user_id = query.value("ID").toInt();
            temp_friend.username = query.value("NAME").toString();
            // temp_friend.isOnline //暂时先不写
            temp_friend.status = friend_status_code::GetStatusCode(query.value("FRIEND_STATUS").toString());
            Logger::getInstance().log(QString("第%1个好友:%2").arg(i++).arg(temp_friend.username));
            friends.push_back(temp_friend);
        }
        QJsonArray friendArray;
        for (auto &temp_friend : friends)
        {
            QJsonObject friendObj;
            friendObj["user_id"] = temp_friend.user_id;
            friendObj["username"] = temp_friend.username;
            friendObj["status"] = friend_status_code::GetStatusCode(temp_friend.status);
            friendArray.append(friendObj);
        }
        obj["friends"] = friendArray;
     
    }
    QJsonDocument doc(obj);
    QString str(doc.toJson(QJsonDocument::Compact));
    m_pProtocol->sendToClient(str, sender);
}

void Core::processChatMessage(QString msg, QWebSocket *sender)
{
    QJsonDocument msgDoc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject msgObj = msgDoc.object();

    // 判断消息类型是否为 "newMessage"
    if (msgObj["type"].toString() == "newChatMessage")
    {
        // 解析消息内容
        int senderId = msgObj["sender_id"].toInt();
        int receiverId = msgObj["receiver_id"].toInt();
        QString content = msgObj["content"].toString();

        Logger::getInstance().log(QString("[消息处理]: 发送者ID:%1 接收者ID:%2 内容:%3")
                                      .arg(senderId)
                                      .arg(receiverId)
                                      .arg(content));

        // 检查接收者是否在线
        QWebSocket *receiverSocket = nullptr;
        for (const auto &client : m_client)
        {
            if (client.user_id == receiverId && client.isOnline)
            {
                receiverSocket = client.socket;
                break;
            }
        }

        if (receiverSocket)
        {
            // 接收者在线，转发消息
            QJsonObject forwardMsg;
            forwardMsg["type"] = "newChatMessage";
            forwardMsg["sender_id"] = senderId;
            forwardMsg["receiver_id"] = receiverId;
            forwardMsg["content"] = content;

            QJsonDocument forwardDoc(forwardMsg);
            QString forwardStr = forwardDoc.toJson(QJsonDocument::Compact);

            m_pProtocol->sendToClient(forwardStr, receiverSocket);

            Logger::getInstance().log("[消息转发成功]: 消息已转发给接收者");
        }
        else
        {
            // 接收者不在线
            Logger::getInstance().log("[消息处理失败]: 接收者不在线");
        }
    }
    else
    {
        Logger::getInstance().log("[消息处理失败]: 未知消息类型");
    }
}


Protocol *Core::getProtocol()
{
    return m_pProtocol;
}

Database *Core::getDatabase()
{
    return m_pDatabase;
}

void Core::onNewMessage(QString msg, QWebSocket *sender)
{
    // Logger::getInstance().log(msg);
    QJsonDocument msg_doc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject msg_obj = msg_doc.object();
    QString type = msg_obj["type"].toString();
    if (type == "login")
    {
        processLogin(msg, sender);
    }
    else if (type == "register")
    {
        processRegister(msg, sender);
    }
    else if (type == "getFriendList")
    {
        processGetFriendList(msg, sender);
    }
    else if (type == "newChatMessage")
    {
        processChatMessage(msg, sender);
    }
    else
    {
        Logger::getInstance().log("未知消息类型");
    }
}

// 数据库结构
/*
-- 创建用户表
CREATE TABLE [dbo].[USERS] (
    [ID]     INT           IDENTITY (1, 1) NOT NULL,-- 主键   自增
    [NAME]   NVARCHAR (50) NOT NULL,-- 用户名
    [PWD]    NVARCHAR (50) NOT NULL,-- 密码
    [STATUS] NVARCHAR (20) CONSTRAINT [DF_Users_Status] DEFAULT ('offline') NOT NULL,-- 状态   默认离线
    CONSTRAINT [PK_Users_ID] PRIMARY KEY CLUSTERED ([ID] ASC),-- 主键
    CONSTRAINT [UQ_Users_Name] UNIQUE NONCLUSTERED ([NAME] ASC),-- 唯一约束 用户名唯一
    CONSTRAINT [CK_Users_Status] CHECK (lower([STATUS])='offline' OR lower([STATUS])='online')-- 约束  状态只能为online或offline
);

-- 好友之间的关系
CREATE TABLE [dbo].[FRIEND_STATUS] (
    [STATUS_CODE] NVARCHAR(20) PRIMARY KEY,
    [DESCRIPTION] NVARCHAR(50) NOT NULL
);

-- 插入初始状态
INSERT INTO [dbo].[FRIEND_STATUS] (STATUS_CODE, DESCRIPTION) VALUES
    ('pending', '待确认'),
    ('accepted', '已接受'),
    ('blocked', '屏蔽');


-- 好友表，用户1与用户2两个外键作为联合主键
CREATE TABLE [dbo].[FRIENDS] (
    [USER1_ID] INT NOT NULL,
    [USER2_ID] INT NOT NULL,
    [STATUS] NVARCHAR(20) NOT NULL,
    CONSTRAINT [PK_Friends] PRIMARY KEY ([USER1_ID], [USER2_ID]),
    CONSTRAINT [FK_Friends_User1] FOREIGN KEY ([USER1_ID]) REFERENCES [dbo].[USERS]([ID]),
    CONSTRAINT [FK_Friends_User2] FOREIGN KEY ([USER2_ID]) REFERENCES [dbo].[USERS]([ID]),
    CONSTRAINT [FK_Friends_Status] FOREIGN KEY ([STATUS]) REFERENCES [dbo].[FRIEND_STATUS]([STATUS_CODE]),
    CONSTRAINT [CK_Friends_Ordered] CHECK ([USER1_ID] < [USER2_ID]) -- 确保 USER1_ID 小于 USER2_ID
);

*/