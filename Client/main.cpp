#include "ui/ClientLogin/ClientLogin.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QTextStream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyle("windowsvista");

    Client w;
    w.show();
    return a.exec();
}
