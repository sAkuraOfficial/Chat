// 聊天主窗口
#pragma once
#include "ui_FriendWidget.h"
#include <QtWidgets/QMainWindow>
#include <qevent.h>
#include <qmovie.h>
class FriendWidget : public QMainWindow
{
    Q_OBJECT
  protected:
  public:
    FriendWidget(QWidget *parent = nullptr);
    ~FriendWidget();

  private:
    Ui::FriendWidget ui;

  private slots:
  public slots:
  signals:
};
