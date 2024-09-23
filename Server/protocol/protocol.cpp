#include "protocol.h"

Protocol::Protocol(QObject *parent)
    : QObject(parent)
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Server"), QWebSocketServer::NonSecureMode, this);
}

Protocol::~Protocol()
{
}

bool Protocol::listen(int port)
{
    if (!m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        log(QString("服务器监听失败 %1").arg(port));
        return false;
    }

    // 处理新连接
    connect(
        m_pWebSocketServer, &QWebSocketServer::newConnection,
        this, &Protocol::onNewConnection
    );

    log(QString("服务器正在监听 %1").arg(port));
    return true;
}

void Protocol::log(QString msg)
{
    emit logMessage(msg + "\n");
}

void Protocol::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    if (!pSocket)
        return; // 无效连接
    log(QString("新连接 %1").arg(pSocket->peerAddress().toString()));
    connect(
        pSocket, &QWebSocket::textMessageReceived,
        this, &Protocol::onTextMessageReceived
    );
    connect(
        pSocket, &QWebSocket::disconnected,
        this, &Protocol::onDisconnected
    );
    m_clients.append(pSocket);
}

void Protocol::onTextMessageReceived(QString msg)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (!pClient)
        return;
    log(QString("收到消息 %1").arg(msg));
    pClient->sendTextMessage(msg + "\t已抄送");
}

void Protocol::onDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (!pClient)
        return;
    log(QString("断开连接 %1").arg(pClient->peerAddress().toString()));
    m_clients.removeAll(pClient);
    pClient->deleteLater();
}