#include "sendmsgtoport.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SendMsgToPort w;
    w.show();
    return a.exec();
}
