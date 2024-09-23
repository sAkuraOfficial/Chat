// 客户端
#pragma once
#include <qdebug.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qwebsocket.h>
class Protocol : public QObject
{
    Q_OBJECT
  public:
    Protocol(QString ws, QObject *parent = nullptr);
    ~Protocol();
    void route(QString message);
    void test(QString msg);
  private:
    QWebSocket *m_pWebSocket;
    QString m_ws;
  private slots:
    void onConnected();
    void onError(QAbstractSocket::SocketError error);
    void onTextMessageReceived(QString message);

};