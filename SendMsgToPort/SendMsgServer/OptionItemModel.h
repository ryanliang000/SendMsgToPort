#ifndef OPTIONMODEL_H
#define OPTIONMODEL_H

#include "qstandarditemmodel.h"
#include "qitemdelegate.h"

struct NodeInfo
{
    NodeInfo* parent;                   // 父节点
    QVector<NodeInfo*> childNodes;      // 子节点
    QString sData;                      // 数据
    int nRow;                           // 所在行
    int nCol;                           // 所在列
    NodeInfo(NodeInfo* p, QString s, int row = -1, int col = -1):
    parent(p), sData(s), nRow(row), nCol(col){}
};

class HistoryModel : public QAbstractItemModel
{
public:
    explicit HistoryModel(QObject *parent = 0);
    virtual ~HistoryModel();

    // 构造父节点下子节点的索引
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    // 通过子节点索引获取父节点索引
    virtual QModelIndex parent(const QModelIndex &child) const override;
    // 获取父节点下子节点的行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // 获取父节点下子节点列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    // 获取节点数据：包括DisplayRole|TextAlignmentRole等
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addReceiveMsg(QString sText);
    void addReplyMsg(QString sText);
    void clearData();
private:
    NodeInfo* m_receiveInfo;
    NodeInfo* m_replyInfo;
    NodeInfo* m_pRootNode;
};


class HistoryDelegate: public QItemDelegate
{
public:
    explicit HistoryDelegate(QObject *parent = 0) : QItemDelegate(parent) {};
};
#endif // OPTIONMODEL_H
