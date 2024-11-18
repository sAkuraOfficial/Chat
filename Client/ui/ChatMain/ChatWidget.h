#pragma once
#include "MessageDelegate.h"
#include <DataTypes/DataTypes.h> // 假设 message_info 和 friend_info 定义在这里
#include <QListView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QWidget>

class ChatWidget : public QWidget
{
    Q_OBJECT
  public:
    explicit ChatWidget(friend_info friend_info, QWidget *parent = nullptr);
    ~ChatWidget();
    void AddMessage(message_info message);

  private:
    friend_info m_friend_info;
    QVBoxLayout *m_vlayout = nullptr;
    QListView *m_list = nullptr;
    QStandardItemModel *m_data_model = nullptr;

};
