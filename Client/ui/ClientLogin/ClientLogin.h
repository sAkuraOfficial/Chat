#pragma once

#include "protocol/protocol.h"
#include "ui_ClientLogin.h"
#include <QtWidgets/QMainWindow>
#include <qevent.h>
#include <qmovie.h>
class Client : public QMainWindow
{
    Q_OBJECT
  protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

  public:
    Client(QWidget *parent = nullptr);
    ~Client();

  private:
    Ui::ClientLoginClass ui;
    Protocol *m_pProtocol = nullptr;
    QMovie *movie;
    QMovie *movie_b;
    bool isLoginPage = true;

  private slots:
    void on_pushButton_login_clicked();
};
