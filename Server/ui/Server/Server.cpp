#include "Server.h"

Server::Server(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_pProtocol = new Protocol(this);
    connect(m_pProtocol, &Protocol::logMessage, this, &Server::recvLog);
    m_pProtocol->listen(1234);//开启监听
}

Server::~Server()
{
}


void Server::recvLog(QString msg)
{
    ui.plainTextEdit_out->appendPlainText(msg);
}