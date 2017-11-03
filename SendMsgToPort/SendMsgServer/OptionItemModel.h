#ifndef OPTIONMODEL_H
#define OPTIONMODEL_H

#include "qstandarditemmodel.h"
#include "qitemdelegate.h"

struct NodeInfo
{
    NodeInfo* parent;                   // ���ڵ�
    QVector<NodeInfo*> childNodes;      // �ӽڵ�
    QString sData;                      // ����
    int nRow;                           // ������
    int nCol;                           // ������
    NodeInfo(NodeInfo* p, QString s, int row = -1, int col = -1):
    parent(p), sData(s), nRow(row), nCol(col){}
};

class HistoryModel : public QAbstractItemModel
{
public:
    explicit HistoryModel(QObject *parent = 0);
    virtual ~HistoryModel();

    // ���츸�ڵ����ӽڵ������
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    // ͨ���ӽڵ�������ȡ���ڵ�����
    virtual QModelIndex parent(const QModelIndex &child) const override;
    // ��ȡ���ڵ����ӽڵ������
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // ��ȡ���ڵ����ӽڵ�����
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    // ��ȡ�ڵ����ݣ�����DisplayRole|TextAlignmentRole��
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
