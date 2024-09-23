#pragma once

#include "protocol/protocol.h"
#include "ui_Client.h"
#include <QtWidgets/QMainWindow>
class Client : public QMainWindow
{
    Q_OBJECT

  public:
    Client(QWidget *parent = nullptr);
    ~Client();

  private:
    Ui::ClientClass ui;
    Protocol *m_pProtocol = nullptr;

  private slots:
    void on_pushButton_clicked();
};
