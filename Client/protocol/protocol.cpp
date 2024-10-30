#include "protocol/protocol.h"
#include "../Logger/logger.h"
#include <qabstractsocket.h>
#include <qtimer.h>
Protocol::Protocol(QObject *parent)
    : QObject(parent)
{
    m_pWebSocket = new QWebSocket();
    connect(m_pWebSocket, &QWebSocket::connected, this, &Protocol::onConnected);
    connect(m_pWebSocket, &QWebSocket::errorOccurred, this, &Protocol::onError);
    connect(m_pWebSocket, &QWebSocket::disconnected, this, &Protocol::onDisconnected);
    connect(m_pWebSocket, &QWebSocket::textMessageReceived, this, &Protocol::onTextMessageReceived);
}

Protocol::~Protocol()
{
    disconnect(m_pWebSocket, nullptr, nullptr, nullptr);
    m_pWebSocket->close();
    m_pWebSocket->deleteLater();
}

QWebSocket *Protocol::getWebSocket()
{
    return m_pWebSocket;
}

void Protocol::ConnectToServer(QString ws)
{
    m_ws = ws;
    m_pWebSocket->open(QUrl(m_ws));
}

void Protocol::onConnected()
{
    Logger::getInstance().log("与服务器建立连接");

}

void Protocol::onError(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::RemoteHostClosedError)
        return;
    Logger::getInstance().log(m_pWebSocket->errorString(), Logger::LogLevel::ERROR);
}

void Protocol::onDisconnected()
{
    Logger::getInstance().log("与服务器断开连接");
}

void Protocol::onTextMessageReceived(QString message)
{
    emit ReceiveNewMessage(message);
}

void Protocol::sendMessage(QString msg)
{
    //Logger::getInstance().log("\n" + msg);
    m_pWebSocket->sendTextMessage(msg);
}