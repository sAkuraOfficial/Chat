#pragma once

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
};
