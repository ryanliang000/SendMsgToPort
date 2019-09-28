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

    // 创建tcp链接
    MyTcpClient* pClient = new MyTcpClient(this);
    connect(pClient, SIGNAL(updateClients(QTcpSocket*, QString)), this, SLOT(updateClients(QTcpSocket*, QString)));
    // 通过接收到的descriptor设置链接
    pClient->setSocketDescriptor(socketDescriptor);
    connect(pClient, SIGNAL(disconnectedFromClient(qintptr)), this, SLOT(slotDisconneted(qintptr)));
    // 放入链接数组中
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

    // 为每个客户端都发送消息
    //for (int i=0; i<m_clientList.count(); i++)
    //{
    //    auto item = m_clientList[i];
    //    if (item->write(bytes.data(), bytes.length()) == bytes.length())
    //    {
    //        emit replyMsg(QString("%1:%2 \r\n%3").arg(item->peerAddress().toString()).arg(item->peerPort()).arg(sReplyMsg));
    //    }
    //}

    // 为该端口返回消息
    if (pSocket->write(bytes.data(), bytes.length()) == bytes.length())
    {
        emit replyMsg(QString("%1:%2 \r\n%3").arg(pSocket->peerAddress().toString()).arg(pSocket->peerPort()).arg(sReplyMsg));
    }

    if (m_sReplyTextSet != "##@@$$")
    {
        // 中断连接
        pSocket->waitForBytesWritten(100);
        pSocket->disconnectFromHost();
    }
}

void MyTcpServer::slotDisconneted(qintptr socketDescriptor)
{
    MyDebugVar2("MyTcpServer::slotDisconneted", socketDescriptor);

    // 链接清除
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
    // 监听端口
    return listen(QHostAddress::Any, m_nPort);
}

void MyTcpServer::stopListen()
{
    // 关闭监听
    if (isListening())
    {
        close();
    }
}
