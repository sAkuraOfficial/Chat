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
    ClientLogin(Core* core, QWidget *parent = nullptr);
    ~ClientLogin();

  private:
    Ui::ClientLoginClass ui;
    Core *m_core = nullptr;
    QMovie *movie;
    QMovie *movie_b;
    bool isLoginPage = true;
    QTimer *animationTimer = nullptr; // 用于加载动画的定时器

  private slots:
    void on_pushButton_login_clicked(); // 登录按钮
    void on_pushButton_reg_clicked();   // 注册按钮
    void onBeginConnect();              // 开始连接,显示加载动画
    void onConnectTimeOut();            // 连接超时
    void onConnectSuccess();            // 连接成功
    void onDisconnected();
    void onReceiveLoginResult(bool result); // 处理登录结果

  signals:
    void LoginSuccess(QString username); // 登陆成功，显示聊天窗口
};
