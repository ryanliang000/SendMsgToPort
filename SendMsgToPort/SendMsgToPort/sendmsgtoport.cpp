#include "sendmsgtoport.h"
#include "qtcpserver.h"
#include "qtcpsocket.h"
#include "TextHexDecode.h"
#include "qhostinfo.h"
#include "qmessagebox.h"

SendMsgToPort::SendMsgToPort(QWidget *parent)
    : QWidget(parent)
{
    // init ui
    ui.setupUi(this);
    ui.comboMsgType->insertItem(0, QString("Text"));
    ui.comboMsgType->insertItem(1, QString("Hex"));
    ui.comboReplyShowType->insertItem(0, QString("Text"));
    ui.comboReplyShowType->insertItem(1, QString("Hex"));
    ui.comboProtocol->insertItem(0, QString("TCP "));
    ui.comboProtocol->insertItem(1, QString("UDP "));
    ui.comboProtocol->insertItem(2, QString("SSL(TCP) "));

    ui.editPort->setText(QString("28881"));
    ui.textMessage->setText(QString::fromUtf8("hello-\xE6\xB4\x9B\xE9\x98\xB3"));

    // init tcp client
    m_pTcpClient = new MyTcpClient();
    m_pSslClient = new MySslClient();

    // init udp client
    m_pUdpClient = new MyUdpClient();

    // bind message
    connect(ui.btnSend, SIGNAL(clicked()), this, SLOT(onSendMessageClick()));
    connect(ui.btnConnect, SIGNAL(clicked()), this, SLOT(onConnect()));
    connect(ui.btnDisconnect, SIGNAL(clicked()), this, SLOT(onDisconnect()));
    connect(ui.comboMsgType, SIGNAL(currentIndexChanged(int)), this, SLOT(onMsgTypeChange()));
    connect(ui.comboReplyShowType, SIGNAL(currentIndexChanged(int)), this, SLOT(onReplyShowTypeChange()));
    connect(ui.comboProtocol, SIGNAL(currentIndexChanged(int)), this, SLOT(onProtocolTypeChange()));
    connect(m_pTcpClient, SIGNAL(updateClients(QString&, QByteArray&)), this, SLOT(onReceiveMessage(QString&, QByteArray&)));
    connect(m_pTcpClient, SIGNAL(disconnectFromServer()), this, SLOT(onDisconnect()));
    connect(m_pSslClient, SIGNAL(updateClients(QString&, QByteArray&)), this, SLOT(onReceiveMessage(QString&, QByteArray&)));
    connect(m_pSslClient, SIGNAL(disconnectFromServer()), this, SLOT(onDisconnect()));
    connect(m_pUdpClient, SIGNAL(updateClients(QString&, QByteArray&)), this, SLOT(onReceiveMessage(QString&, QByteArray&)));
    updateBtnState(false);

    // get local ip, and find the first one
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress address, info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol) 
        {
            ui.editIP->setText(address.toString());
            break;
        }
    }
}

SendMsgToPort::~SendMsgToPort()
{
    // 删除连接
    if (m_pTcpClient)
    {
        m_pTcpClient->disconnectFromHost();
        delete m_pTcpClient;
    }
    m_pTcpClient = nullptr;
    if (m_pSslClient)
    {
        m_pSslClient->disconnectFromHost();
        delete m_pSslClient;
    }
    m_pSslClient = nullptr;

    if (m_pUdpClient)
    {
        delete m_pUdpClient;
    }
    m_pUdpClient = nullptr;
}

void SendMsgToPort::sendSslMsg(QByteArray sBytes)
{
    if (!m_pSslClient->waitForConnected(1000))
    {
       MyDebugVar1("SendMsgToPort::sendSslMsg Fail --- connect abnormal!")
       QMessageBox::information(this, "info", "Connect abnormal!");
       ui.btnSend->setDisabled(true);
       ui.btnConnect->setDisabled(false);
       return;
    }
    m_pSslClient->resetReceiveData();
    if (m_pSslClient->write(sBytes.data(), sBytes.length()))
    {
        MyDebugVar2("SendMsgToPort::sendSslMsg Succ", QString::fromStdString(sBytes.toStdString()));
        return;
    }
    MyDebugVar1("SendMsgToPort::sendSslMsg Fail!");
    QMessageBox::information(this, "info", "Send message to port fail!");
}

void SendMsgToPort::sendTcpMsg(QByteArray sBytes)
{
    if (!m_pTcpClient->waitForConnected(1000))
    {
       MyDebugVar1("SendMsgToPort::sendTcpMsg Fail --- connect abnormal!")
       QMessageBox::information(this, "info", "Connect abnormal!");
       ui.btnSend->setDisabled(true);
       ui.btnConnect->setDisabled(false);
       return;
    }
    m_pTcpClient->resetReceiveData();
    if (m_pTcpClient->write(sBytes.data(), sBytes.length()))
    {
        MyDebugVar2("SendMsgToPort::sendTcpMsg Succ", QString::fromStdString(sBytes.toStdString()));
        return;
    }
    MyDebugVar1("SendMsgToPort::onTcpMsg Fail!");
    QMessageBox::information(this, "info", "Send message to port fail!");
}
void SendMsgToPort::sendUdpMsg(QByteArray sBytes)
{
    // close last connection
    m_pUdpClient->close();

    QString sIP = ui.editIP->text();
    QString sPort = ui.editPort->text();

    //m_pUdpClient->bind(QHostAddress(sIP), sPort.toInt());
    m_pUdpClient->connectToHost(sIP, sPort.toInt());

    for (int i=0; i<10; i++)
        if (m_pUdpClient->write(sBytes.data(), sBytes.length()))
        {
            MyDebugVar2("SendMsgToPort::sendUdpMsg Succ", QString::fromStdString(sBytes.toStdString()))
            return;
        }

    MyDebugVar1("SendMsgToPort::sendUdpMsg Fail!");
    QMessageBox::information(this, "info", "Send message to port fail!");
}

void SendMsgToPort::onSendMessageClick()
{
    QString sTextMsgContent = ui.textMessage->toPlainText();
    ui.textReply->clear();

    QByteArray sBytes;
    switch(ui.comboMsgType->currentIndex())
    {
    case 0: // text
        {
            sBytes = sTextMsgContent.toUtf8();
        }
        break;
    case 1:
        {
            CTextHexDecode decode;
            QString sText = decode.utf8HexToText(sTextMsgContent);
            sBytes = sText.toUtf8();
        }
        break;
    }

    bool bIsReplaceNewLine = ui.checkForNewLine->isChecked();
    if (bIsReplaceNewLine)
    {
        sBytes = sBytes.replace("\r\n", "\n");
        sBytes = sBytes.replace("\n", "\r\n");
    }

    if (ui.comboProtocol->currentIndex() == 0)
    {
        sendTcpMsg(sBytes);
    }
    else if(ui.comboProtocol->currentIndex() == 1)
    {
        sendUdpMsg(sBytes);
    }
    else
    {
        sendSslMsg(sBytes);
    }
}

void SendMsgToPort::onReceiveMessage(QString& msg, QByteArray& orgBtes)
{
    m_sLastReceiveMsg = msg;
    m_sLastReceiveOrgBtes = orgBtes;
    onReplyShowTypeChange();
}

void SendMsgToPort::onConnect()
{
    QString sIP = ui.editIP->text();
    QString sPort = ui.editPort->text();
    ui.textReply->clear();

    switch(ui.comboProtocol->currentIndex())
    {
        case 0: // tcp
        {
            m_pTcpClient->connectToHost(sIP, sPort.toUInt(), QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);
            if (m_pTcpClient->waitForConnected(1000))
            {
                updateBtnState(true);
                MyDebugVar3("SendMsgToPort::onConnect Succ!", sIP, sPort)
                return;
            }
        }
        break;
        case 2: // Ssl
        {
            m_pSslClient->connectToHostEncrypted(sIP, sPort.toUInt(), QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);
            if (m_pSslClient->waitForEncrypted(1000))
            {
                updateBtnState(true);
                MyDebugVar3("SendMsgToPort::onConnect Succ!", sIP, sPort)
                return;
            }
            else {
                MyDebugVar1(m_pSslClient->errorString());
            }
        }
        break;
    }


    MyDebugVar3("SendMsgToPort::onConnect Fail!", sIP, sPort)
    QMessageBox::information(this, "info", "Connect to server failed!");
}

void SendMsgToPort::onDisconnect()
{
    // 删除连接
    m_pTcpClient->disconnectFromHost();
    m_pSslClient->disconnectFromHost();
    updateBtnState(false);
}

void SendMsgToPort::updateBtnState(bool bIsConnect)
{
    ui.btnConnect->setDisabled(bIsConnect);
    ui.btnDisconnect->setDisabled(!bIsConnect);
    ui.btnSend->setDisabled(!bIsConnect);
}

void SendMsgToPort::onProtocolTypeChange()
{
    switch(ui.comboProtocol->currentIndex())
    {
    case 0: // tcp
    case 2: // ssl
        {
            m_pTcpClient->close();
            m_pSslClient->close();
            ui.btnConnect->setEnabled(true);
            ui.btnDisconnect->setEnabled(false);
            ui.btnSend->setEnabled(false);
        }
        break;
    case 1: // udp
        {
            m_pUdpClient->close();
            ui.btnConnect->setEnabled(false);
            ui.btnDisconnect->setEnabled(false);
            ui.btnSend->setEnabled(true);
        }
        break;
    }
}
void SendMsgToPort::onMsgTypeChange()
{
    return;
}
void SendMsgToPort::onReplyShowTypeChange()
{
    switch(ui.comboReplyShowType->currentIndex())
    {
    case 0: // text
        {
            ui.textReply->setPlainText(m_sLastReceiveMsg);
        }
        break;
    case 1: // hex
        {
            CTextHexDecode decode;
            QString sText = decode.byteArrayToHex(m_sLastReceiveOrgBtes);
            ui.textReply->setPlainText(sText);
        }
        break;
    }
}
