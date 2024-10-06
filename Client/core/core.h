#pragma once
// 客户端内核类,作为protocol与ui的中间层
#include "protocol/protocol.h"
#include <qobject.h>

class Core : public QObject
{
    Q_OBJECT
  public:
    Core(QObject *parent = nullptr);
    ~Core();

    void runClient(QString ws, int timeoutMs); // 第二个参数：连接超时时间
    Protocol *getProtocol();
    bool isConnecting();
    void login(QString username, QString password);

  private:
    Protocol *m_pProtocol = nullptr;
    QTimer *timer = nullptr;

  private slots:
    void onNewMessage(QString message);
  signals:
    void beginConnect();   // 发送开始连接信号，用于ui显示加载动画
    void ConnectTimeOut(); // 连接超时
    void ConnectSuccess(); // 连接成功
};