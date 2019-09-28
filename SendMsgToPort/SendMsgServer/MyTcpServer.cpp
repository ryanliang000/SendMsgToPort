#include "MyTcpServer.h"
#include "MyTcpClient.h"

MyTcpServer::MyTcpServer(QObject *parent /*= 0*/, int port)
    : QTcpServer(parent)
    , m_nPort(port)
{
    setReplyTextSameWithReceive();
}


MyTcpServer::~MyTcpServer(void)
{

}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    MyDebugVar2("MyTcpServer::incomingConnection", socketDescriptor);

    // ����tcp����
    MyTcpClient* pClient = new MyTcpClient(this);
    connect(pClient, SIGNAL(updateClients(QTcpSocket*, QString)), this, SLOT(updateClients(QTcpSocket*, QString)));
    // ͨ�����յ���descriptor��������
    pClient->setSocketDescriptor(socketDescriptor);
    connect(pClient, SIGNAL(disconnectedFromClient(qintptr)), this, SLOT(slotDisconneted(qintptr)));
    // ��������������
    m_clientList.append(pClient);

}

void MyTcpServer::updateClients(QTcpSocket* pSocket, QString msg)
{
    MyDebugVar2("MyTcpServer::updateClients", msg);
    emit receivedMsg(QString("%1:%2 \r\n%3").arg(pSocket->peerAddress().toString()).arg(pSocket->peerPort()).arg(msg));

    QString sReplyMsg;
    if (m_sReplyTextSet == "##@@$$")
    {
        sReplyMsg = msg;
    }
    else
    {
        sReplyMsg = m_sReplyTextSet;
    }
    sReplyMsg.replace("\r\n", "\n");
    sReplyMsg.replace("\n", "\r\n");
    QByteArray bytes = sReplyMsg.toUtf8();

    // Ϊÿ���ͻ��˶�������Ϣ
    //for (int i=0; i<m_clientList.count(); i++)
    //{
    //    auto item = m_clientList[i];
    //    if (item->write(bytes.data(), bytes.length()) == bytes.length())
    //    {
    //        emit replyMsg(QString("%1:%2 \r\n%3").arg(item->peerAddress().toString()).arg(item->peerPort()).arg(sReplyMsg));
    //    }
    //}

    // Ϊ�ö˿ڷ�����Ϣ
    if (pSocket->write(bytes.data(), bytes.length()) == bytes.length())
    {
        emit replyMsg(QString("%1:%2 \r\n%3").arg(pSocket->peerAddress().toString()).arg(pSocket->peerPort()).arg(sReplyMsg));
    }

    if (m_sReplyTextSet != "##@@$$")
    {
        // �ж�����
        pSocket->waitForBytesWritten(100);
        pSocket->disconnectFromHost();
    }
}

void MyTcpServer::slotDisconneted(qintptr socketDescriptor)
{
    MyDebugVar2("MyTcpServer::slotDisconneted", socketDescriptor);

    // �������
    for (int i=0; i<m_clientList.count(); i++)
    {
        QTcpSocket* item = m_clientList[i];
        if (item->socketDescriptor() == socketDescriptor)
        {
            m_clientList[i]->disconnectFromHost();
            m_clientList.removeAt(i);
            return;
        }
    }
}

bool MyTcpServer::startListen()
{
    // �����˿�
    return listen(QHostAddress::Any, m_nPort);
}

void MyTcpServer::stopListen()
{
    // �رռ���
    if (isListening())
    {
        close();
    }
}
