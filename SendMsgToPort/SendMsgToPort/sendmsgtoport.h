#ifndef SENDMSGTOPORT_H
#define SENDMSGTOPORT_H

#include <QtWidgets/QWidget>
#include "ui_sendmsgtoport.h"
#include "MyTcpClient.h"
#include "MyUdpClient.h"
#include "qbytearray.h"

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
    void onMsgTypeChange();
    void onProtocolTypeChange();

private:
    void sendTcpMsg(QByteArray sBytes);
    void sendUdpMsg(QByteArray sBytes);
    void sendSslMsg(QByteArray sBytes);

private:
    Ui::SendMsgToPortClass ui;
    MyTcpClient* m_pTcpClient;
    MySslClient* m_pSslClient;
    MyUdpClient* m_pUdpClient;
    QString m_sLastReceiveMsg;
    QByteArray m_sLastReceiveOrgBtes;
};

#endif // SENDMSGTOPORT_H
