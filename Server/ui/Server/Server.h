#pragma once

#include "protocol/protocol.h"
#include "ui_Server.h"
#include <QWebSocketServer>
#include <QtWidgets/QMainWindow>
#include <qwebsocket.h>

class Server : public QMainWindow
{
    Q_OBJECT

  public:
    Server(QWidget *parent = nullptr);
    ~Server();

  private:
    Ui::ServerClass ui;
    Protocol *m_pProtocol = nullptr;

  private slots:
    void recvLog(QString msg);

  signals:
    void newConnection();
};
