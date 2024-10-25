// 聊天主窗口
#include "ClientMain.h"
#include "../ClientLogin/ClientLogin.h"
#include <QTimer>
ClientMain::ClientMain(Core *core,  QWidget *parent)
    : QMainWindow(parent), m_core(core)
{
    ui.setupUi(this);
}

ClientMain::~ClientMain()
{
  
}

void ClientMain::onLoginSuccess(QString username)
{
    m_username = username;
    this->show();
    this->setWindowTitle("欢迎您，" + m_username);
}