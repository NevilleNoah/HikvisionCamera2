#ifndef TREECHOOSE_H
#define TREECHOOSE_H

#include "database.h"
#include <QDialog>
#include <QString>
#include <QTreeWidget>

namespace Ui {
class TreeChoose;
}

class TreeChoose : public QDialog
{
    Q_OBJECT

public:
    explicit TreeChoose(QWidget *parent = nullptr);
    ~TreeChoose();
    void initDatabase();
    void initUI();
    static Database database;

private:
    Ui::TreeChoose *ui;
    TreeChoose* treechoose;
    QTreeWidget *tree;

    QTreeWidgetItem* addTreeRoot(QString name, int colNum);
    QTreeWidgetItem* addTreeChild(QTreeWidgetItem *parent, QString name, int colNum);
};

#endif // TREECHOOSE_H
