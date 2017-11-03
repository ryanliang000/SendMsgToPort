#include "OptionItemModel.h"


HistoryModel::HistoryModel(QObject *parent /*= 0*/)
    : QAbstractItemModel(parent)
{
    // 创建root节点
    m_pRootNode = new NodeInfo(nullptr, "rootNode", -1, -1);
    m_receiveInfo = new NodeInfo(m_pRootNode, "ReceiveMessage", 0, 0);
    m_replyInfo = new NodeInfo(m_pRootNode, "ReplyMessage", 1, 0);
    m_pRootNode->childNodes.append(m_receiveInfo);
    m_pRootNode->childNodes.append(m_replyInfo);
}

// 递归删除信息
void EraseNode(NodeInfo* pNode)
{
    if (pNode != nullptr)
    {
        for (int i=0; i<pNode->childNodes.count(); i++)
        {
            EraseNode(pNode->childNodes[i]);
        }
        delete pNode;
    }
}
HistoryModel::~HistoryModel()
{
    EraseNode(m_pRootNode);
    m_pRootNode = nullptr;
    m_receiveInfo = nullptr;
    m_replyInfo = nullptr;
}
QModelIndex HistoryModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (parent.row() == -1 && parent.column() == -1)
    {
        // 首层节点绑定关系
        if (row < m_pRootNode->childNodes.count())
            return createIndex(row, column, m_pRootNode->childNodes[row]);
    }
    else
    {
        // 其它层节点绑定关系
        if (parent.internalPointer() != nullptr)
        {
            NodeInfo* pNode = reinterpret_cast<NodeInfo*>(parent.internalPointer());
            if (pNode->childNodes.size() > row)
            {
                return createIndex(row, column, pNode->childNodes[row]);
            } 
        }
    }
    return QModelIndex();
}

QModelIndex HistoryModel::parent(const QModelIndex &child) const
{
    if (child.internalPointer() != nullptr)
    {
        NodeInfo* pNode = reinterpret_cast<NodeInfo*>(child.internalPointer());
        NodeInfo* pParent = pNode->parent;
        if (pParent != nullptr)
        {
            // 根据父节点信息：row/col/node*获取Index
            return createIndex(pParent->nRow, pParent->nCol, pParent);;
        }
    }
    return QModelIndex();
}
int HistoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.internalPointer() == nullptr)
    {
        // 根节点下的数据行数
        return m_pRootNode->childNodes.count();
    }
    else
    {
        // 节点下的数据行数
        NodeInfo* pNode = reinterpret_cast<NodeInfo*>(parent.internalPointer());
        return pNode->childNodes.size();
    }
}
int HistoryModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    // 每行有量列
    return 1;
}

QVariant HistoryModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    // 节点内容：左对齐，显示行列号
    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter); 
    }
    else if (role == Qt::DisplayRole) 
    {
        if (index.internalPointer() == 0)
        {
            return QString("row=%1,col=%2").arg(index.row()).arg(index.column());
        }
        else
        {
            NodeInfo* pNode = reinterpret_cast<NodeInfo*>(index.internalPointer());
            return pNode->sData;
        }
    }
    else
    {
        return QVariant();
    }
}

void HistoryModel::addReceiveMsg(QString sText)
{
    beginResetModel();
    int nRow = m_receiveInfo->childNodes.count();
    m_receiveInfo->childNodes.append(new NodeInfo(m_receiveInfo, sText, nRow, 0));
    endResetModel(); 
}
void HistoryModel::addReplyMsg(QString sText)
{
    beginResetModel();
    int nRow = m_replyInfo->childNodes.count();
    m_replyInfo->childNodes.append(new NodeInfo(m_replyInfo, sText, nRow, 0));
    endResetModel();  
}

void HistoryModel::clearData()
{
    beginResetModel();
    for (int i=0; i<m_receiveInfo->childNodes.count(); i++)
    {
        EraseNode(m_receiveInfo->childNodes[i]);
    }
    m_receiveInfo->childNodes.clear();
    for (int i=0; i<m_replyInfo->childNodes.count(); i++)
    {
        EraseNode(m_replyInfo->childNodes[i]);

    }
    m_replyInfo->childNodes.clear();
    endResetModel();
}



//QVariant HistoryModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
//{
//    if (role != Qt::DisplayRole)
//        return QVariant();
//
//    //横标头显示“设置描述”、“设置选项”等字样
//    if (orientation == Qt::Horizontal)
//    {
//        switch (section)
//        {
//        case 0:
//            return QString("column1");
//        case 1:
//            return QString("column2");
//        default:
//            return QString("column-unknown");
//        }
//    }
//
//    //竖标题显示行号
//    if (orientation == Qt::Vertical)
//        return section + 1;
//
//    return QVariant();
//}

