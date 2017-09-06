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
    ui.editPort->setText(QString("28881"));
    ui.textMessage->setText(QString::fromLocal8Bit("china-洛阳"));

    // init tcp client
    m_pClient = new MyTcpClient();

    // bind message
    connect(ui.btnSend, SIGNAL(clicked()), this, SLOT(onSendMessageClick()));
    connect(ui.btnConnect, SIGNAL(clicked()), this, SLOT(onConnect()));
    connect(ui.btnDisconnect, SIGNAL(clicked()), this, SLOT(onDisconnect()));
    connect(m_pClient, SIGNAL(updateClients(QString&, QByteArray&)), this, SLOT(onReceiveMessage(QString&, QByteArray&)));
    connect(ui.comboReplyShowType, SIGNAL(currentIndexChanged(int)), this, SLOT(onReplyShowTypeChange()));
    connect(m_pClient, SIGNAL(disconnectFromServer()), this, SLOT(onDisconnect()));
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
    if (m_pClient)
    {
        m_pClient->disconnectFromHost();
        delete m_pClient;
    }
}


void SendMsgToPort::onSendMessageClick()
{
    if (!m_pClient->waitForConnected(1000))
    {
       MyDebugVar1("SendMsgToPort::onSendMessageClick Fail --- connect abnormal!")
       QMessageBox::information(this, "info", "Connect abnormal!");
       ui.btnSend->setDisabled(true);
       ui.btnConnect->setDisabled(false);
       return;
    }
    QString sTextMsgContent = ui.textMessage->toPlainText();

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
    
    if (m_pClient->write(sBytes.data(), sBytes.length()))
    {
        MyDebugVar2("SendMsgToPort::onSendMessageClick Succ", sTextMsgContent)
        return;
    }
    MyDebugVar1("SendMsgToPort::onSendMessageClick Fail!");
    QMessageBox::information(this, "info", "Send message to port fail!");
    
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
    m_pClient->connectToHost(sIP, sPort.toUInt(), QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);

    if (m_pClient->waitForConnected(1000))
    {
        updateBtnState(true);
        MyDebugVar3("SendMsgToPort::onConnect Succ!", sIP, sPort)
        return;
    }

    MyDebugVar3("SendMsgToPort::onConnect Fail!", sIP, sPort)
    QMessageBox::information(this, "info", "Connect to server failed!");
}

void SendMsgToPort::onDisconnect()
{
    // 删除连接
    if (m_pClient)
    {
        m_pClient->disconnectFromHost();
        updateBtnState(false);
    }
}

void SendMsgToPort::updateBtnState(bool bIsConnect)
{
    ui.btnConnect->setDisabled(bIsConnect);
    ui.btnDisconnect->setDisabled(!bIsConnect);
    ui.btnSend->setDisabled(!bIsConnect);
}

void SendMsgToPort::onReplyShowTypeChange()
{
    switch(ui.comboReplyShowType->currentIndex())
    {
    case 0: // text
        {
            ui.textReply->setText(m_sLastReceiveMsg);
        }
        break;
    case 1:
        {
            CTextHexDecode decode;
            QString sText = decode.byteArrayToHex(m_sLastReceiveOrgBtes);
            ui.textReply->setText(sText);
        }
        break;
    }
}
