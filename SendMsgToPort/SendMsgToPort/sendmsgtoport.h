#ifndef SENDMSGTOPORT_H
#define SENDMSGTOPORT_H

#include <QtWidgets/QWidget>
#include "ui_sendmsgtoport.h"
#include "MyTcpClient.h"

class SendMsgToPort : public QWidget
{
    Q_OBJECT

public:
    SendMsgToPort(QWidget *parent = 0);
    ~SendMsgToPort();

    void updateBtnState(bool bIsConnect);
public slots:
    void onSendMessageClick();
    void onReceiveMessage(QString& msg, QByteArray& orgBytes);
    void onConnect();
    void onDisconnect();
    void onReplyShowTypeChange();

private:
    Ui::SendMsgToPortClass ui;
    MyTcpClient* m_pClient;
    QString m_sLastReceiveMsg;
    QByteArray m_sLastReceiveOrgBtes;
};

#endif // SENDMSGTOPORT_H
