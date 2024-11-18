#pragma once

#include "../Logger/logger.h"
#include "core/core.h"
#include "protocol/protocol.h"
#include "ui_ServerControler.h"
#include <QStandardItemModel> // 引入标准模型头文件
#include <QtWidgets/QMainWindow>

class ServerControler : public QMainWindow
{
    Q_OBJECT

  public:
    ServerControler(QWidget *parent = nullptr);
    ~ServerControler();

  private:
    Ui::ServerControlerClass ui;
    Core *m_server = nullptr;

    QStandardItemModel *m_userListModel = nullptr; // 模型用于管理用户列表
    QModelIndex m_currentIndex; // 存储当前右键点击的项索引

  private slots:
    void recvLog(QString msg);
    void onClientLogin(client_info client);
    void onUserListContextMenu(const QPoint &pos);
    void setAsSender();
    void setAsReceiver();
    void on_pushButton_send_msg_clicked();


  signals:
    void newConnection();
};
