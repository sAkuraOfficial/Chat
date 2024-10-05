#pragma once
#include <QObject>
#include <database/database.h>
#include <protocol/protocol.h>
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

  private:
    Protocol *m_pProtocol = nullptr;
    Database *m_pDatabase = nullptr;
    void processLogin(QString msg, QWebSocket *sender);
    void processRegister(QString msg, QWebSocket *sender);

  private slots:
    void onNewMessage(QString msg, QWebSocket *sender);
  signals:
};