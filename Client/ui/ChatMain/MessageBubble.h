#pragma once
#include <DataTypes/DataTypes.h>
#include<qlabel.h>
class MessageBubble : public QLabel
{
    Q_OBJECT
  protected:
  public:
    MessageBubble(message_info message_info, bool isClient, QWidget *parent = nullptr); // isClient用于判断是不是自己
    ~MessageBubble();

  private:
    message_info m_message_info;
  private slots:
  public slots:
  signals:
};
