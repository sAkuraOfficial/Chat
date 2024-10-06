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
    QTimer *animationTimer = nullptr;//���ڼ��ض����Ķ�ʱ��


  private slots:
    void on_pushButton_login_clicked();
    void onBeginConnect(); // ��ʼ����,��ʾ���ض���
    void onConnectTimeOut(); // ���ӳ�ʱ
    void onConnectSuccess(); // ���ӳɹ�
    void onDisconnected();
};
