// 聊天主窗口
#pragma once
#include "ChatMessageList.h"
#include "ChatWidget.h"
#include "ui_ClientMain.h"
#include <DataTypes/DataTypes.h>
#include <QtWidgets/QMainWindow>
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
    user_info GetClientUserInfo();

  private:
    Ui::ClientMain ui;
    Core *m_core = nullptr;
    user_info m_client_user_info;
    QVector<friend_info> m_friends;
    QMap<int, ChatWidget *> m_chatWidgets; // 用于跟踪用户ID和ChatWidget的映射关系

  private slots:
    void onReceiveGetFriendList(QVector<friend_info> friends);
    void onSendMessageToServer(message_info message);
    void on_listWidget_friends_itemClicked(QListWidgetItem *item);
  public slots:
    void onLoginSuccess(QString username, int user_id);
    void onReceiveUserMessage(message_info message);
};
