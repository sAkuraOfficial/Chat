#pragma once
#include <QObject>
#include <database/database.h>
#include <protocol/protocol.h>
#include <qmessagebox.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
class Server : public QObject
{
    Q_OBJECT
  public:
    Server(QObject *parent = nullptr);
    ~Server();
    Protocol *getProtocol();
    Database *getDatabase();
    bool runServer(int port);

  private:
    Protocol *m_pProtocol = nullptr;
    Database *m_pDatabase = nullptr;
  private slots:
  signals:
};