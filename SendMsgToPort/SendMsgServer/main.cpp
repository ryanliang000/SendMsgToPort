#include "sendmsgserver.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SendMsgServer w;
    w.show();
    return a.exec();
}
