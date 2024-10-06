#pragma once
#include "ui_ClientLogin.h"
#include <QtWidgets/QMainWindow>
#include <core/core.h>
#include <qevent.h>
#include <qmovie.h>
class ClientLogin : public QMainWindow
{
    Q_OBJECT
  protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

  public:
    ClientLogin(QWidget *parent = nullptr);
    ~ClientLogin();

  private:
    Ui::ClientLoginClass ui;
    Core *m_core = nullptr;
    QMovie *movie;
    QMovie *movie_b;
    bool isLoginPage = true;
    QTimer *animationTimer = nullptr;//用于加载动画的定时器


  private slots:
    void on_pushButton_login_clicked();
    void onBeginConnect(); // 开始连接,显示加载动画
    void onConnectTimeOut(); // 连接超时
    void onConnectSuccess(); // 连接成功
    void onDisconnected();
};
