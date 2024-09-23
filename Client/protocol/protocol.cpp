#include "protocol/protocol.h"

Protocol::Protocol(QString ws, QObject *parent)
    : m_ws(ws), QObject(parent)
{
    m_pWebSocket = new QWebSocket();
    connect(m_pWebSocket, &QWebSocket::connected, this, &Protocol::onConnected);
    connect(m_pWebSocket, &QWebSocket::errorOccurred, this, &Protocol::onError);
    m_pWebSocket->open(QUrl(m_ws));
}

Protocol::~Protocol()
{
}

void Protocol::onConnected()
{
    connect(
        m_pWebSocket, &QWebSocket::textMessageReceived,
        this, &Protocol::onTextMessageReceived
    );
}

void Protocol::onError(QAbstractSocket::SocketError error)
{
    QMessageBox::warning(nullptr, "Error", m_pWebSocket->errorString());
}

void Protocol::onTextMessageReceived(QString message)
{
    route(message);
}

void Protocol::route(QString message)
{
    // 先暂时弹窗处理
    QMessageBox::warning(nullptr, "从服务端收到Message", message);
}

void Protocol::test(QString msg)
{
    m_pWebSocket->sendTextMessage(msg);
}