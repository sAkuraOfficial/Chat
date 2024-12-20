#pragma once
// 客户端内核类,作为protocol与ui的中间层
#include "protocol/protocol.h"
#include <DataTypes/DataTypes.h>
#include <qobject.h>
class Core : public QObject
{
    Q_OBJECT
  public:
    Core(QObject *parent = nullptr);
    ~Core();
    Protocol *getProtocol();
    bool isConnecting();
    void setClientUserInfo(user_info* client_user_info);//仅可由ClientMain设置
    void runClient(QString ws, int timeoutMs); // 第二个参数：连接超时时间

    void login(QString username, QString password);
    void registerUser(QString username, QString password);
    void getFriendList(int user_id);
    void sendMessage(message_info message);

    // 处理接收到的消息,msg是接收到的json消息
    void processLogin(QJsonObject msg_json);
    void processRegister(QJsonObject msg_json);
    void processGetFriendList(QJsonObject msg_json);
    void processNewMessage(QJsonObject msg_json);

  private:
    Protocol *m_pProtocol = nullptr;
    QTimer *timer = nullptr;
    user_info* m_client_user_info;

  private slots:
    void onReceiveNewMessage(QString message);
  signals:
    void beginConnect();                               // 发送开始连接信号，用于ui显示加载动画
    void ConnectTimeOut();                             // 连接超时
    void ConnectSuccess();                             // 连接成功
    void ReceiveLoginResult(bool result, int user_id); // 登录结果
    void ReceiveRegisterResult(bool result);           // 注册结果
    void ReceiveGetFriendList(QVector<friend_info> friends);
    void ReceiveUserMessage(message_info message);
};