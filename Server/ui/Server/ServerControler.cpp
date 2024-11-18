#include "ServerControler.h"
#include <QAction>
#include <QMenu>
#include <QRegularExpression>
#include <qjsondocument.h>
#include <qjsonobject.h>
ServerControler::ServerControler(QWidget *parent)
    : QMainWindow(parent)
{
    connect(&Logger::getInstance(), &Logger::newLog_Formated, this, &ServerControler::recvLog);
    ui.setupUi(this);

    Logger::getInstance().log("ServerControler::ServerControler(QWidget *parent)");

    m_server = new Core(this);
    m_server->runServer(1234);

    connect(m_server, &Core::clientLogin, this, &ServerControler::onClientLogin); // 新客户端登录信号

    // 初始化模型并绑定到 QListView
    m_userListModel = new QStandardItemModel(this);
    ui.listView_userList->setModel(m_userListModel);

    // 设置 QListView 右键菜单策略
    ui.listView_userList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.listView_userList, &QListView::customContextMenuRequested, this, &ServerControler::onUserListContextMenu);
}

ServerControler::~ServerControler()
{
}

void ServerControler::recvLog(QString msg)
{
    ui.plainTextEdit_out->appendPlainText(msg);
}

void ServerControler::onClientLogin(client_info client)
{
    // 格式化为“昵称（用户ID）”的形式
    QString clientInfo = QString("%1（%2）")
                             .arg(client.username)
                             .arg(client.user_id);

    // 创建新的列表项
    QStandardItem *item = new QStandardItem(clientInfo);
    QFont font = item->font();
    font.setPointSize(14); // 设置字体大小，比如 14
    item->setFont(font);
    // 添加到模型
    m_userListModel->appendRow(item);

    // 日志记录
    Logger::getInstance().log(QString("New client connected: %1").arg(clientInfo));
}

void ServerControler::onUserListContextMenu(const QPoint &pos)
{
    // 获取鼠标点击位置的项索引
    QModelIndex index = ui.listView_userList->indexAt(pos);
    if (!index.isValid())
    {
        return; // 如果没有点击有效项，则返回
    }

    // 创建右键菜单
    QMenu contextMenu(this);

    // 创建“设为发送者”选项
    QAction *setSenderAction = new QAction(tr("设为发送者"), this);
    connect(setSenderAction, &QAction::triggered, this, &ServerControler::setAsSender);

    // 创建“设为接收者”选项
    QAction *setReceiverAction = new QAction(tr("设为接收者"), this);
    connect(setReceiverAction, &QAction::triggered, this, &ServerControler::setAsReceiver);

    // 将选项添加到右键菜单
    contextMenu.addAction(setSenderAction);
    contextMenu.addAction(setReceiverAction);

    // 记录当前点击的项索引
    m_currentIndex = index;

    // 显示右键菜单
    contextMenu.exec(ui.listView_userList->viewport()->mapToGlobal(pos));
}

void ServerControler::setAsSender()
{
    if (!m_currentIndex.isValid())
    {
        return; // 如果当前索引无效，则返回
    }

    // 获取当前项的内容（已是“昵称（用户ID）”格式）
    QString clientInfo = m_userListModel->itemFromIndex(m_currentIndex)->text();

    // 将选中的项设为发送者
    ui.lineEdit_sender->setText(clientInfo);

    // 记录日志
    Logger::getInstance().log(QString("Client set as sender: %1").arg(clientInfo));
}

void ServerControler::setAsReceiver()
{
    if (!m_currentIndex.isValid())
    {
        return; // 如果当前索引无效，则返回
    }

    // 获取当前项的内容（已是“昵称（用户ID）”格式）
    QString clientInfo = m_userListModel->itemFromIndex(m_currentIndex)->text();

    // 将选中的项设为接收者
    ui.lineEdit_receiver->setText(clientInfo);

    // 记录日志
    Logger::getInstance().log(QString("Client set as receiver: %1").arg(clientInfo));
}
void ServerControler::on_pushButton_send_msg_clicked()
{
    // 获取发送者和接收者信息
    QString senderText = ui.lineEdit_sender->text();
    QString receiverText = ui.lineEdit_receiver->text();
    QString message = ui.textEdit_send_msg->toPlainText(); // 获取要发送的消息

    if (senderText.isEmpty() || receiverText.isEmpty())
    {
        Logger::getInstance().log("发送者或接收者为空", Logger::ERROR);
        QMessageBox::warning(this, "发送失败", "请先选择发送者和接收者！");
        return;
    }

    // 解析出发送者和接收者的用户ID
    QRegularExpression regex("（(\\d+)）"); // 匹配“（数字）”的部分，以提取 ID
    QRegularExpressionMatch match;

    int sender_id = -1, receiver_id = -1;

    // 提取发送者ID
    match = regex.match(senderText);
    if (match.hasMatch())
    {
        sender_id = match.captured(1).toInt();
    }

    // 提取接收者ID
    match = regex.match(receiverText);
    if (match.hasMatch())
    {
        receiver_id = match.captured(1).toInt();
    }

    if (sender_id == -1 || receiver_id == -1)
    {
        Logger::getInstance().log("无法解析发送者或接收者ID", Logger::ERROR);
        QMessageBox::warning(this, "发送失败", "发送者或接收者的ID无效！");
        return;
    }

    // 找到发送者对应的 WebSocket 连接
    QWebSocket *senderSocket = nullptr;
    for (const client_info &client : m_server->getClientList())
    {
        if (client.user_id == sender_id)
        {
            senderSocket = client.socket;
            break;
        }
    }

    if (!senderSocket)
    {
        Logger::getInstance().log("无法找到发送者的连接", Logger::ERROR);
        QMessageBox::warning(this, "发送失败", "发送者未连接！");
        return;
    }

    // 构建 JSON 消息
    QJsonObject msgObj;
    msgObj["type"] = "newMessage";       // 修改消息类型为 "newMessage"
    msgObj["sender_id"] = sender_id;     // 发送者ID
    msgObj["receiver_id"] = receiver_id; // 接收者ID
    msgObj["content"] = message;         // 消息内容

    QJsonDocument doc(msgObj);
    QString jsonString = doc.toJson(QJsonDocument::Compact);

    // 使用 Core 的协议类发送消息
    m_server->getProtocol()->sendToClient(jsonString, senderSocket);
    Logger::getInstance().log(QString("消息已发送: 发送者ID:%1 接收者ID:%2 内容:%3").arg(sender_id).arg(receiver_id).arg(message));
}
