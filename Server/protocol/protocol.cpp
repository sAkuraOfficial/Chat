#include "protocol.h"

Protocol::Protocol(QObject *parent)
    : QObject(parent)
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Server"), QWebSocketServer::NonSecureMode, this);
}

Protocol::~Protocol()
{
    for (QWebSocket *client : m_clients)
    {
        disconnect(client, nullptr, nullptr, nullptr); // 断开所有信号与槽的连接
        client->deleteLater();                         // 标记为待删除
    }
    m_clients.clear(); // 清空客户端列表

    // 关闭服务器
    if (m_pWebSocketServer)
    {
        m_pWebSocketServer->close();       // 停止监听新连接
        m_pWebSocketServer->deleteLater(); // 标记服务器对象为待删除
    }
}

bool Protocol::listen(int port)
{
    if (!m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        return false;
    }

    // 处理新连接
    connect(
        m_pWebSocketServer, &QWebSocketServer::newConnection,
        this, &Protocol::onNewConnection
    );
    return true;
}

void Protocol::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    if (!pSocket)
        return; // 无效连接
    Logger::getInstance().log(QString("新连接 %1").arg(pSocket->peerAddress().toString()));
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
    router(msg, pClient);
}

void Protocol::router(QString msg, QWebSocket *sender)
{
    emit newClientMessage(msg, sender);
}

void Protocol::sendToClient(QString msg, QWebSocket *client)
{
    client->sendTextMessage(msg);
}

void Protocol::onDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (!pClient)
        return;
    Logger::getInstance().log(QString("断开连接 %1").arg(pClient->peerAddress().toString()));
    disconnect(pClient, nullptr, nullptr, nullptr); // 断开所有信号与槽的连接
    m_clients.removeAll(pClient);
    pClient->close(); // 确保连接关闭
    delete pClient;
}
