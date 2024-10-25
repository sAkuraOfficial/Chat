// 服务端
#pragma once
#include "../Logger/logger.h"
#include <QObject>
#include <QWebSocketServer>
#include <qmessagebox.h>
#include <qsqldatabase.h>
#include <qwebsocket.h>
class Protocol : public QObject
{
    Q_OBJECT
  public:
    Protocol(QObject *parent = nullptr);
    ~Protocol();
    bool listen(int port);

    void router(QString msg, QWebSocket *sender);

    void sendToClient(QString msg, QWebSocket *client); // 发送消息给客户端

  private:
    QWebSocketServer *m_pWebSocketServer = nullptr;
    QVector<QWebSocket *> m_clients;
  private slots:
    void onNewConnection();
    void onTextMessageReceived(QString msg);
    void onDisconnected();
  signals:
    void newClientMessage(QString msg, QWebSocket *sender); // 当收到客户端消息时发射此信号
};