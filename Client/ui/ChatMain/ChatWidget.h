#pragma once
#include "OneMessage.h"
#include <DataTypes/DataTypes.h>
#include <QtWidgets/QMainWindow>
#include <qlayout.h>
#include <qlistview.h>
#include <qstandarditemmodel.h>
class ChatWidget : public QWidget
{
    Q_OBJECT
  protected:
  public:
    ChatWidget(friend_info friend_info, QWidget *parent = nullptr);
    ~ChatWidget();
    void AddMessage(message_info message);

  private:
    friend_info m_friend_info;
    QVBoxLayout *m_vlayout = nullptr;
    QListView *m_list = nullptr;
    QStandardItemModel *m_data_model = nullptr;
  private slots:
  public slots:
  signals:
};
