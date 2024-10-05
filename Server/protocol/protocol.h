// 服务端
#pragma once
#include <QObject>
#include <QWebSocketServer>
#include <qmessagebox.h>
#include <qsqldatabase.h>
#include <qwebsocket.h>
#include"logger/logger.h"
class Protocol : public QObject
{
    Q_OBJECT
  public:
    Protocol(QObject *parent = nullptr);
    ~Protocol();
    bool listen(int port);

    void router(QString msg, QWebSocket *sender);

  private:
    QWebSocketServer *m_pWebSocketServer = nullptr;
    QVector<QWebSocket *> m_clients;
  private slots:
    void onNewConnection();
    void onTextMessageReceived(QString msg);
    void onDisconnected();

};