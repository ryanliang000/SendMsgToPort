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
    qDebug("MyTcpClient::onDataReceived");
    m_sReceivedData.clear();

    int nLen = bytesAvailable();
    QByteArray sBytes;
    if (nLen <= 1024 * 1024)
    {
        sBytes = readAll();
        m_sReceivedData = QString::fromUtf8(sBytes, nLen);

    }
    else
    {
        reset();
        m_sReceivedData = QString("Error: message to long (above 1 mb)!");
    }
    emit updateClients(m_sReceivedData, sBytes);
}

void MyTcpClient::onDisconnected()
{
    qDebug("MyTcpClient::onDisconnected");
    emit disconnectFromServer();
}

void MyTcpClient::onConnected()
{
    qDebug("MyTcpClient::onConnected");
}
