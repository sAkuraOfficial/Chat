#include "core.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
Core::Core(QObject *parent)
    : QObject(parent)
{
    m_pDatabase = new Database(this);
    m_pProtocol = new Protocol(this);
}
Core::~Core()
{
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
}

void Core::processRegister(QString msg, QWebSocket *sender)
{
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