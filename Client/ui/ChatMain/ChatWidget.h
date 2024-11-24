#pragma once
#include "ChatMessageList.h"
#include "ui_ChatWidget.h"
#include <DataTypes/DataTypes.h>
#include <QtWidgets/QMainWindow>
#include <qlayout.h>
/*
---------------------------编码规范---------------------------
ChatWidget是聊天小部件，它包含了会话名、消息列表、小工具、消息编辑、发送按钮
在ClientMain中，有一个堆栈小部件stackwidget，用于存放所有的ChatWidget
当用户点击不同的好友，就会切换或创建到对应的ChatWidget
--------------------------------------------------------------
*/
class ChatWidget : public QWidget
{
    Q_OBJECT
  protected:
  public:
    ChatWidget(friend_info friend_info,user_info client_user_info,QWidget *parent = nullptr);
    ~ChatWidget();
    void addMessage(message_info message);
    void setSessionName(QString session_name);//手动更改会话名

  private:
    ChatMessageList *m_messageList = nullptr;
    Ui::ChatWidget ui;
    friend_info m_friend_info;
    user_info m_client_user_info;
  private slots:
    void on_pushButton_sendMessage_clicked();
  public slots:
signals:
    void SendMessageToServer(message_info message);//需要在ClientMain绑定
};
