#pragma once
#include <DataTypes/DataTypes.h>
#include<qlabel.h>
class MessageBubble : public QLabel
{
    Q_OBJECT
  protected:
  public:
    MessageBubble(message_info message_info, bool isClient, QWidget *parent = nullptr); // isClient�����ж��ǲ����Լ�
    ~MessageBubble();

  private:
    message_info m_message_info;
  private slots:
  public slots:
  signals:
};
