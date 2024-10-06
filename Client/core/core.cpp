#include "core.h"
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

void Core::onNewMessage(QString message)
{

}

void Core::login(QString username, QString password)
{
    QJsonObject json;
    json["type"] = "login";
    json["username"] = username;
    json["password"] = password;
    QJsonDocument doc(json);
    m_pProtocol->sendMessage(doc.toJson());

}