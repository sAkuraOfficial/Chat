#include "core.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
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
        QJsonObject obj;
        obj["type"] = "login";
        if (query.next())
        {
            Logger::getInstance().log("登录成功");
            obj["result"] = true;
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
    if (query.lastError().isValid())
    {
        Logger::getInstance().log("注册失败");
    }
    else
    {
        Logger::getInstance().log("注册成功");
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
    else
    {
        Logger::getInstance().log("未知消息类型");
    }
}

// 数据库结构
/*
CREATE TABLE[dbo].[MSG](
    [ID] INT IDENTITY(1, 1) NOT NULL, --主键 自增[SENDER] INT NOT NULL, --发送者[CONTENT] NVARCHAR(MAX) NOT NULL, --内容 无限长度[SEND_TIME] DATETIME2(7) CONSTRAINT[DF_MSG_SENDTIME] DEFAULT(getdate()) NOT NULL, --发送时间 默认当前时间 CONSTRAINT[PK_MSG] PRIMARY KEY CLUSTERED([ID] ASC), --主键 CONSTRAINT[FK_MSG_USER] FOREIGN KEY([SENDER]) REFERENCES[dbo].[USERS]([ID]) ON DELETE CASCADE ON UPDATE CASCADE-- 外键 SEND_ID 关联USER_ID
);

CREATE TABLE[dbo].[USERS](
    [ID] INT IDENTITY(1, 1) NOT NULL, --主键 自增[NAME] NVARCHAR(50) NOT NULL, --用户名[PWD] NVARCHAR(50) NOT NULL, --密码[STATUS] NVARCHAR(20) CONSTRAINT[DF_Users_Status] DEFAULT('offline') NOT NULL, --状态 默认离线 CONSTRAINT[PK_Users_ID] PRIMARY KEY CLUSTERED([ID] ASC), --主键 CONSTRAINT[UQ_Users_Name] UNIQUE NONCLUSTERED([NAME] ASC), --唯一约束 用户名唯一 CONSTRAINT[CK_Users_Status] CHECK(lower([STATUS]) = 'offline' OR lower([STATUS]) = 'online')-- 约束 状态只能为online或offline
);

*/