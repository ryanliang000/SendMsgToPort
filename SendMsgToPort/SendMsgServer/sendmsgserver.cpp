#include "sendmsgserver.h"
#include "qapplication.h"
#include "qdesktopwidget.h"
#include "qscrollbar.h"

SendMsgServer::SendMsgServer(QWidget *parent)
    : QMainWindow(parent)
    , m_pTcpServer(nullptr)
{
    ui.setupUi(this);

    // 分割信息设置
    m_pMainWidget = ui.leftWidget;
    QSplitter* pSpliter = new QSplitter(Qt::Horizontal, this);
    
    m_pOptionForm = new OptionForm(this);
    pSpliter->addWidget(m_pOptionForm);
    m_pOptionForm->setVisible(false);

    pSpliter->addWidget(m_pMainWidget);
    
    // 连接到central widget
    ui.mainLayout->addWidget(pSpliter);

    connect(ui.btnStart, SIGNAL(clicked()), this, SLOT(onStartServer()));
    connect(ui.btnStop, SIGNAL(clicked()), this, SLOT(onStopServer()));
    connect(ui.btnClear, SIGNAL(clicked()), this, SLOT(onClearContent()));
    connect(ui.btnOption, SIGNAL(clicked()), this, SLOT(onOptionChange()));
    connect(m_pOptionForm, SIGNAL(visibleChange()), this, SLOT(onOptionChange()));
    connect(ui.textRplySet, SIGNAL(textChanged()), this, SLOT(onReplySetTextChange()));
    connect(ui.checkReplySet, SIGNAL(clicked()), this, SLOT(onCheckReplySetClick()));

    ui.textRplySet->setDisabled(ui.checkReplySet->isChecked());
    ui.btnOption->setText("<---History");
    ui.btnStop->setDisabled(true);
    ui.btnStart->setDisabled(false);

    ui.editPort->setText("28881");
}

SendMsgServer::~SendMsgServer()
{

}

void SendMsgServer::onStartServer()
{
    if (m_pTcpServer == nullptr)
    {
        uint nPort = ui.editPort->text().toUInt();
        m_pTcpServer = new MyTcpServer(this, nPort);
        connect(m_pTcpServer, SIGNAL(receivedMsg(QString)), this, SLOT(onReceivedMsg(QString)));
        connect(m_pTcpServer, SIGNAL(replyMsg(QString)), this, SLOT(onReplyMsg(QString)));
        connect(m_pTcpServer, SIGNAL(receivedMsg(QString)), m_pOptionForm, SLOT(onAddReceiveMessage(QString)));
        connect(m_pTcpServer, SIGNAL(replyMsg(QString)), m_pOptionForm, SLOT(onAddReplyMessage(QString)));
        onReplySetTextChange();

        // 启动链接
        if (m_pTcpServer->startListen())
        {
            // 按钮重置
            ui.btnStart->setDisabled(true);
            ui.btnStop->setDisabled(false);
            MyDebugVar2("SendMsgServer::onStartServer Succ!", nPort);
        }
        else
        {
            MyDebugVar2("SendMsgServer::onStartServer Fail!", nPort);
        }
    }
}

void SendMsgServer::onStopServer()
{
    MyDebugVar1("SendMsgServer::onStopServer!");
    if (m_pTcpServer != nullptr)
    {
        // 关闭删除连接
        m_pTcpServer->stopListen();
        delete m_pTcpServer;
        m_pTcpServer = nullptr;
        
        // 按钮重置
        ui.btnStop->setDisabled(true);
        ui.btnStart->setDisabled(false);
    }
}

void SendMsgServer::onClearContent()
{
    ui.textReceive->setText("");
    ui.textReply->setText("");
}

void SendMsgServer::onReceivedMsg(QString msg)
{
    ui.textReceive->append("Receive: " + msg);

    QScrollBar *pScroll = ui.textReceive->verticalScrollBar();
    if (pScroll)
    {
        pScroll->setSliderPosition(pScroll->maximum());
    }
}


void SendMsgServer::onReplyMsg(QString msg)
{
    ui.textReply->append("Reply: " + msg);
    QScrollBar *pScroll = ui.textReply->verticalScrollBar();
    if (pScroll)
    {
        pScroll->setSliderPosition(pScroll->maximum());
    }
}

void SendMsgServer::onOptionChange()
{
    // 获取宽度
    int nWidth = this->size().width();
    static int c_nFixedOptionWidth = 500;

    if (m_pOptionForm->isVisible())
    {
        ui.btnOption->setText("<---History");
        m_pOptionForm->setVisible(false);

        // 宽度调宽
        this->setFixedWidth(nWidth - c_nFixedOptionWidth);
        m_pOptionForm->setFixedWidth(c_nFixedOptionWidth);

        //移动到最中间
        this->move(pos().x()+c_nFixedOptionWidth, pos().y());
    }
    else
    {
        ui.btnOption->setText("-->History");
        m_pOptionForm->setVisible(true);

        // 宽度调窄
        this->setFixedWidth(nWidth + c_nFixedOptionWidth);
        m_pOptionForm->setFixedWidth(c_nFixedOptionWidth);

        //移动到最中间
        this->move(pos().x()-c_nFixedOptionWidth, pos().y());
    }

}

void SendMsgServer::onReplySetTextChange()
{
    if (m_pTcpServer != nullptr)
    {
        if (!ui.checkReplySet->isChecked())
        {
            m_pTcpServer->setReplyText(ui.textRplySet->toPlainText());
        }
        else
        {
            m_pTcpServer->setReplyTextSameWithReceive();
        }
    }
    
}

void SendMsgServer::onCheckReplySetClick()
{
    ui.textRplySet->setDisabled(ui.checkReplySet->isChecked());
    onReplySetTextChange();
}
