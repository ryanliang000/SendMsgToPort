#include "MyTcpClient.h"

MyTcpClient::MyTcpClient(QObject *parent) 
    : QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(onDataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
}


MyTcpClient::~MyTcpClient(void)
{

}


void MyTcpClient::onDataReceived()
{
    int nLen = bytesAvailable();
    qDebug(QString("MyTcpClient::onDataReceived len=%1").arg(nLen).toUtf8());

    if (nLen <= 1024 * 1024 * 100)
    {
        m_sReceiveBytes += readAll();
        m_sReceivedData = QString::fromUtf8(m_sReceiveBytes, m_sReceiveBytes.length());
    }
    else
    {
        reset();
        m_sReceivedData = QString("Error: message to long (above 100 mb)!");
    }
    emit updateClients(m_sReceivedData, m_sReceiveBytes);
}

void MyTcpClient::onDisconnected()
{
    qDebug("MyTcpClient::onDisconnected");
    emit disconnectFromServer();
}

void MyTcpClient::onConnected()
{
    m_sReceivedData.clear();
    m_sReceiveBytes.clear();
    qDebug("MyTcpClient::onConnected");
}


MySslClient::MySslClient(QObject *parent)
    : QSslSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(onDataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(this, SIGNAL(encrypted()), this, SLOT(onEncrypted()));
    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(this, SIGNAL(peerVerifyError(QSslError)), this, SLOT(onPeerVerifyError(QSslError)));
    connect(this, SIGNAL(sslErrors(const QList<QSslError>)), this, SLOT(onSslErrors(const QList<QSslError>)));
    addCaCertificates("/Users/ryan/Documents/sockpro/ssl/cert.pem");
}

void MySslClient::onDataReceived()
{
    int nLen = bytesAvailable();
    qDebug(QString("MySslClient::onDataReceived len=%1").arg(nLen).toUtf8());

    if (nLen <= 1024 * 1024 * 100)
    {
        m_sReceiveBytes += readAll();
        m_sReceivedData = QString::fromUtf8(m_sReceiveBytes, m_sReceiveBytes.length());
    }
    else
    {
        reset();
        m_sReceivedData = QString("Error: message to long (above 100 mb)!");
    }
    emit updateClients(m_sReceivedData, m_sReceiveBytes);
}

void MySslClient::onDisconnected()
{
    qDebug("MySslClient::onDisconnected");
    emit disconnectFromServer();
}

void MySslClient::onConnected()
{
    m_sReceivedData.clear();
    m_sReceiveBytes.clear();
    qDebug("MySslClient::onConnected");
}

void MySslClient::onPeerVerifyError(QSslError error)
{
    MyDebugVar1("MySslClient::onPeerVerifyError");
    MyDebugVar1(error.errorString());
}

void MySslClient::onSslErrors(const QList<QSslError> lstErrors)
{
    MyDebugVar1("MySslClient::onSslErrors: do ignoreSslErrors");
    ignoreSslErrors(lstErrors);
}

void MySslClient::onEncrypted()
{
    MyDebugVar1("MySslClient::onEncrypted");
}

void MySslClient::onReadyRead()
{
    MyDebugVar1("MySslClient::onReadyRead");
}
