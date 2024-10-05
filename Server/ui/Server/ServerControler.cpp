#include "ServerControler.h"
ServerControler::ServerControler(QWidget *parent)
    : QMainWindow(parent)
{
    connect(&Logger::getInstance(), &Logger::newLog_Formated, this, &ServerControler::recvLog);
    ui.setupUi(this);
    Logger::getInstance().log("ServerControler::ServerControler(QWidget *parent)");
    m_server = new Core(this);
    m_server->runServer(1234);
}

ServerControler::~ServerControler()
{
}

void ServerControler::recvLog(QString msg)
{
    ui.plainTextEdit_out->appendPlainText(msg);
}