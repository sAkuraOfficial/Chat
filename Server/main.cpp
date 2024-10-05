#include "ui/Server/ServerControler.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerControler w;
    w.show();
    return a.exec();
}
