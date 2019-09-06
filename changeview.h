﻿#ifndef CHANGEVIEW_H
#define CHANGEVIEW_H

#include "database.h"
#include <QDialog>
#include "treechoose.h"



namespace Ui {
class ChangeView;
}

class ChangeView : public QDialog
{
    Q_OBJECT


    enum OperateType
    {
        Add ,
        Edit
    };

public:
    explicit ChangeView(QWidget *parent = nullptr);
    ~ChangeView();
    void setEditData(QStringList rowData);
    static QDateTime dateEdit;
private:
    Ui::ChangeView *ui;
    static Database database;
    static QList<RECORD> records;
    void clearData();
    void closeEvent(QCloseEvent *event);
    OperateType operate;
    TreeChoose* treechoose;

signals:
    //    void signalInfo(QVariantMap Info);

public slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void Exec(OperateType operateType);

private slots:
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
};

#endif // CHANGEVIEW_H
