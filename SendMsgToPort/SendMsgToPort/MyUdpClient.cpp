#include "MyUdpClient.h"

MyUdpClient::MyUdpClient(QObject *parent)
    :QUdpSocket (parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(onDataReceived()));
}


void MyUdpClient::onDataReceived()
{
    qDebug("MyUdpClient::onDataReceived");
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
