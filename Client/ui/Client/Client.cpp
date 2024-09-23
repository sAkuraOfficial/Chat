#include "Client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_pProtocol = new Protocol("ws://localhost:1234", this);
}

Client::~Client()
{
}

void Client::on_pushButton_clicked()
{
    m_pProtocol->test(ui.lineEdit->text());
}