// 服务端
#pragma once
#include <QObject>
#include <QWebSocketServer>
#include <qmessagebox.h>
#include <qwebsocket.h>
class Protocol : public QObject
{
    Q_OBJECT
  public:
    Protocol(QObject *parent = nullptr);
    ~Protocol();
    bool listen(int port);
    void log(QString msg);

  private:
    QWebSocketServer *m_pWebSocketServer = nullptr;
    QVector<QWebSocket *> m_clients;
  private slots:
    void onNewConnection();
    void onTextMessageReceived(QString msg);
    void onDisconnected();
  signals:
    void logMessage(QString msg);
};