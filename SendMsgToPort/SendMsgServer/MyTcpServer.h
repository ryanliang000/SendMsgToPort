#pragma once
#include "qtcpserver.h"
#include "MyTcpClient.h"
class MyTcpServer :
    public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0, int port = 28888);
    ~MyTcpServer(void);

    bool startListen();
    void stopListen();
    void setReplyText(QString sText) {m_sReplyTextSet = sText;}
    void setReplyTextSameWithReceive() {m_sReplyTextSet = "##@@$$";}

signals:
    void receivedMsg(QString);
    void replyMsg(QString);

public slots:
    void updateClients(QTcpSocket*, QString);
    void slotDisconneted(qintptr);

protected:
    void incomingConnection(int socketDescriptor);

private:
    int m_nPort;
    QList<QTcpSocket*> m_clientList;
    QString m_sReplyTextSet;
};

