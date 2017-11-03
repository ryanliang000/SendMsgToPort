#ifndef SENDMSGSERVER_H
#define SENDMSGSERVER_H

#include <QtWidgets/QMainWindow>
#include "ui_sendmsgserver.h"
#include "MyTcpServer.h"
#include "optionform.h"
#include "qsplitter.h"

class SendMsgServer : public QMainWindow
{
    Q_OBJECT

public:
    SendMsgServer(QWidget *parent = 0);
    ~SendMsgServer();

public slots:
    void onStartServer();
    void onStopServer();
    void onClearContent();
    void onReceivedMsg(QString);
    void onReplyMsg(QString);
    void onOptionChange();
    void onReplySetTextChange();
    void onCheckReplySetClick();
private:
    Ui::SendMsgServerClass ui;
    MyTcpServer* m_pTcpServer;
    OptionForm* m_pOptionForm;
    QWidget* m_pMainWidget;
};

#endif // SENDMSGSERVER_H
