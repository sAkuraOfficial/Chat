#pragma once
#include "MessageDelegate.h"
#include <DataTypes/DataTypes.h> // 假设 message_info 和 friend_info 定义在这里
#include <QListView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QWidget>
/*
---------------------------编码规范---------------------------
ChatMessageList在ChatWidget中被使用
ChatMessageList是消息列表，用于存储、显示用户与单个好友的聊天记录
使用了委托MessageDelegate，用于自定义消息的显示
使用了标准模型QStandardItemModel，用于存储消息数据
--------------------------------------------------------------
*/
class ChatMessageList : public QWidget
{
    Q_OBJECT
  public:
    explicit ChatMessageList(friend_info friend_info, QWidget *parent = nullptr);
    ~ChatMessageList();
    void AddMessage(message_info message);
    int getFriendId();

  protected:
    void wheelEvent(QWheelEvent *event) override; // 重写滚轮事件，支持平滑滚动

  private:
    friend_info m_friend_info;
    QVBoxLayout *m_vlayout = nullptr;
    QListView *m_list = nullptr;
    QStandardItemModel *m_data_model = nullptr;
};
