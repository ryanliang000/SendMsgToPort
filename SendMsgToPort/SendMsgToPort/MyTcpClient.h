#pragma once
#include "qtcpsocket.h"
#include "qsslsocket.h"
#define  MyDebugVar1(desc1) {QString sDebugInfo = QString("%1").arg(desc1); qDebug(sDebugInfo.toUtf8());}
#define  MyDebugVar2(desc1, desc2) {QString sDebugInfo = QString("%1, %2").arg(desc1).arg(desc2); qDebug(sDebugInfo.toUtf8());}
#define  MyDebugVar3(desc1, desc2, desc3) {QString sDebugInfo = QString("%1, %2, %3").arg(desc1).arg(desc2).arg(desc3); qDebug(sDebugInfo.toUtf8());}
class MyTcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpClient(QObject *parent = nullptr);
    ~MyTcpClient(void);

    void resetReceiveData()
    {
        m_sReceivedData.clear();
        m_sReceiveBytes.clear();
    }

public slots:
    void onDataReceived();
    void onDisconnected();
    void onConnected();
signals:
    void updateClients(QString&, QByteArray&);
    void disconnectFromServer();

private:
    QString m_sReceivedData;
    QByteArray m_sReceiveBytes;
};

class MySslClient : public QSslSocket
{
    Q_OBJECT
public:
    explicit MySslClient(QObject *parent = nullptr);

    void resetReceiveData()
    {
        m_sReceivedData.clear();
        m_sReceiveBytes.clear();
    }

public slots:
    void onDataReceived();
    void onDisconnected();
    void onConnected();
    void onPeerVerifyError(QSslError error);
    void onSslErrors(const QList<QSslError>);
    void onEncrypted();
    void onReadyRead();

signals:
    void updateClients(QString&, QByteArray&);
    void disconnectFromServer();

private:
    QString m_sReceivedData;
    QByteArray m_sReceiveBytes;
};

