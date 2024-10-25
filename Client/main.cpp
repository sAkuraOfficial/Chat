#include "core/core.h"
#include "ui/ChatMain/ClientMain.h"
#include "ui/ClientLogin/ClientLogin.h"
#include <QFile>
#include <QTextStream>
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     a.setStyle("windowsvista"); // 设置风格

    // 创建核心对象
    Core *core = new Core(); 

    // 创建登录窗口和主窗口
    ClientLogin *clientLogin = new ClientLogin(core);
    ClientMain *clientMain = new ClientMain(core);
    QObject::connect(clientLogin, &ClientLogin::LoginSuccess, clientMain, &ClientMain::onLoginSuccess);

    clientLogin->show();

    return a.exec();
}
