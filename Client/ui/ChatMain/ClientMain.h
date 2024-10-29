// 聊天主窗口
#pragma once
#include "ui_ClientMain.h"
#include <QtWidgets/QMainWindow>
#include <DataTypes/DataTypes.h>
#include <core/core.h>
#include <qevent.h>
#include <qmovie.h>
#include <qvector.h>
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
    QVector<friend_info> m_friends;

  private slots:
    void onReceiveGetFriendList(QVector<friend_info> friends);
  public slots:
    void onLoginSuccess(QString username, int user_id);
};
