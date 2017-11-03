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
    if (nLen <= 1024 * 1024)
    {
        QByteArray sBytes = readAll();
        m_sReceivedData = QString::fromUtf8(sBytes, nLen);
        
    }
    else
    {
        reset();
        m_sReceivedData = QString("Error: message to long (above 1 mb)!");
    }
    emit updateClients(this, m_sReceivedData);
}

void MyTcpClient::onDisconnected()
{
    qDebug("MyTcpClient::onDisconnected");
    emit disconnectedFromClient(socketDescriptor());
}

void MyTcpClient::onConnected()
{
    qDebug("MyTcpClient::onConnected");
}
