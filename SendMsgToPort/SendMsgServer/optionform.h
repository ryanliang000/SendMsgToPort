#ifndef OPTIONFORM_H
#define OPTIONFORM_H

#include <QObject>
#include "ui_OptionForm.h"
#include "qdialog.h"
#include "OptionItemModel.h"

class OptionForm : public QDialog
{
    Q_OBJECT

public:
    OptionForm(QWidget *parent);
    ~OptionForm();

signals:
    void visibleChange();

public slots:
     void onOk();
     void onClear();
     void onAddReceiveMessage(QString sData);
     void onAddReplyMessage(QString sData);

private:
    Ui::dlgOption ui;
    HistoryModel* m_pModel;
};

#endif // OPTIONFORM_H
