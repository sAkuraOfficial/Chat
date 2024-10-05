#pragma once

#include "protocol/protocol.h"
#include "ui_ServerControler.h"
#include <QtWidgets/QMainWindow>
#include"core/core.h"
#include"../Logger/logger.h"
class ServerControler : public QMainWindow
{
    Q_OBJECT

  public:
    ServerControler(QWidget *parent = nullptr);
    ~ServerControler();

  private:
    Ui::ServerControlerClass ui;
    Core* m_server=nullptr;

  private slots:
    void recvLog(QString msg);

  signals:
    void newConnection();
};
