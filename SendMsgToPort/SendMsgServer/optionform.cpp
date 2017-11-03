#include "optionform.h"

OptionForm::OptionForm(QWidget *pWidget)
    : QDialog(pWidget)
{
    ui.setupUi(this);
    connect(ui.btnOK, SIGNAL(clicked()), this, SLOT(onOk()));
    connect(ui.btnClear, SIGNAL(clicked()), this, SLOT(onClear()));
    connect(ui.treeView, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(onTreeClick()));

    m_pModel = new HistoryModel(this);
    ui.treeView->setModel(m_pModel);
    ui.treeView->setHeaderHidden(true);
    ui.treeView->setExpanded(QModelIndex(), true);

    //QStandardItemModel* pModel = new QStandardItemModel(this);
    //{
    //    // 创建首层节点
    //    QStandardItem* topItem = new QStandardItem();
    //    topItem->setData(QString("Root"), Qt::DisplayRole);
    //    pModel->appendRow(topItem);

    //    for (int i = 0; i < 3;i++)
    //    {
    //        // 创建首层下的二层节点
    //        QList<QStandardItem*> items;
    //        QStandardItem* item = new QStandardItem("123");
    //        items.push_back(item);
    //        topItem->appendRow(items);

    //        for (int j=0; j< 3; j++)
    //        {
    //            // 创建二层下的三层节点
    //            QList<QStandardItem*> subItems;
    //            QStandardItem* subItem = new QStandardItem("456");
    //            subItems.push_back(subItem);
    //            item->appendRow(subItems);
    //        }
    //    }
    //}
    //ui.treeView->setModel(pModel);

    HistoryDelegate* pDelegate = new HistoryDelegate(this);
    ui.treeView->setItemDelegate(pDelegate);
    ui.treeView->update();
}

OptionForm::~OptionForm()
{

}

void OptionForm::onOk()
{
    emit visibleChange();
}

void OptionForm::onClear()
{
    m_pModel->clearData();
}

void OptionForm::onAddReceiveMessage(QString sData)
{
    m_pModel->addReceiveMsg(sData);
    ui.treeView->expandAll();
}

void OptionForm::onAddReplyMessage(QString sData)
{
    m_pModel->addReplyMsg(sData);
    ui.treeView->expandAll();
}
