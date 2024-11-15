#pragma once
#include "Avatar.h"
#include "MessageBubble.h"
#include <DataTypes/DataTypes.h>
#include <QtWidgets/QMainWindow>
#include <qlayout.h>
class OneMessage : public QWidget
{
    Q_OBJECT
  protected:
  public:
    OneMessage(message_info message_info, bool isClient, QWidget *parent = nullptr); // isClient�����ж��ǲ����Լ�
    ~OneMessage();

  private:
    message_info m_message_info;
    QHBoxLayout *m_hlayout;
    Avatar *m_avatar;
    MessageBubble *m_messageBubble;
  private slots:
  public slots:
  signals:
};
