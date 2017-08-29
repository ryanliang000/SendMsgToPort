#pragma once
#include "qtcpsocket.h"
#define  MyDebugVar1(desc1) {QString sDebugInfo = QString("%1").arg(desc1); qDebug(sDebugInfo.toUtf8());}
#define  MyDebugVar2(desc1, desc2) {QString sDebugInfo = QString("%1, %2").arg(desc1).arg(desc2); qDebug(sDebugInfo.toUtf8());}
#define  MyDebugVar3(desc1, desc2, desc3) {QString sDebugInfo = QString("%1, %2, %3").arg(desc1).arg(desc2).arg(desc3); qDebug(sDebugInfo.toUtf8());}
class MyTcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpClient(QObject *parent = 0);
    ~MyTcpClient(void);

    QString getReceiveText()
    {
        return m_sReceivedData;
    }

public slots:
    void onDataReceived();
    void onDisconnected();
    void onConnected();
signals:
    void updateClients(QString);

private:
    QString m_sReceivedData;
};

