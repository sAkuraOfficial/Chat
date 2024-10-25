// 聊天主窗口
#pragma once
#include "ui_ClientMain.h"
#include <QtWidgets/QMainWindow>
#include <core/core.h>
#include <qevent.h>
#include <qmovie.h>
class ClientMain : public QMainWindow
{
    Q_OBJECT
  protected:
  public:
    ClientMain(Core *core, QWidget *parent = nullptr);
    ~ClientMain();

  private:
    Ui::ClientMain ui;
    Core *m_core = nullptr;
    QString m_username;

  private slots:
  public slots:
    void onLoginSuccess(QString username);
};
