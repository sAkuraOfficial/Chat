#pragma once
#include "DataTypes/DataTypes.h"
#include "database/database.h"
#include "protocol/protocol.h"
#include <QObject>
#include <qmessagebox.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
class Core : public QObject
{
    Q_OBJECT
  public:
    Core(QObject *parent = nullptr);
    ~Core();
    Protocol *getProtocol();
    Database *getDatabase();
    bool runServer(int port);
    QVector<client_info> getClientList();

  private:
    Protocol *m_pProtocol = nullptr;
    Database *m_pDatabase = nullptr;
    QVector<client_info> m_client;
    // msg是json消息，sender是发送者
    void processLogin(QString msg, QWebSocket *sender);
    void processRegister(QString msg, QWebSocket *sender);
    void processGetFriendList(QString msg, QWebSocket *sender);

  private slots:
    void onNewMessage(QString msg, QWebSocket *sender);
  signals:
    void clientLogin(client_info client);
};