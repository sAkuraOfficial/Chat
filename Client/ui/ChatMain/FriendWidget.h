// 聊天主窗口
#pragma once
#include <DataTypes/DataTypes.h>
#include "ui_FriendWidget.h"
#include <QtWidgets/QMainWindow>
class FriendWidget : public QMainWindow
{
    Q_OBJECT
  protected:
  public:
    FriendWidget(friend_info friend_info, QWidget *parent = nullptr);
    ~FriendWidget();

  private:
    Ui::FriendWidget ui;
    friend_info m_friend_info;
  private slots:
  public slots:
  signals:
};
