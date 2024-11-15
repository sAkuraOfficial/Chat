#pragma once
#include <DataTypes/DataTypes.h>
#include <QtWidgets/QMainWindow>
#include <qlabel.h>
class Avatar : public QWidget
{
    Q_OBJECT
  protected:
  public:
    Avatar(user_info user_info, QWidget *parent = nullptr); // isClient用于判断是不是自己
    ~Avatar();

  private:
    user_info m_user_info;
    QLabel *m_avatar;

  private slots:
  public slots:
  signals:
};
