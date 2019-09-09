#include "treechoose.h"
#include "ui_treechoose.h"
#include "database.h"
Database TreeChoose::database;

TreeChoose::TreeChoose(QWidget *parent) : QDialog(parent),ui(new Ui::TreeChoose)
{
    ui->setupUi(this);
    initUI();
}

TreeChoose::~TreeChoose()
{
    delete ui;
}

//初始化UI设置
void TreeChoose::initUI() {
    QString community[1] = { QString::fromLocal8Bit("新兴民院") }; //小区
    QString building[5] = { "1", "2", "3", "4", "5" }; //幢
    QString unit[5] = { "1", "2", "3", "4", "5" }; //单元
    QString house[4] = { "101", "102", "201", "202" };
    for(int i = 0; i < 1; ++i) {
        QTreeWidgetItem *p1 = addTreeRoot(community[i], 0);
        for(int j = 0; j < 5; ++j) {
            QTreeWidgetItem *p2 = addTreeChild(p1, building[j], 1);
            for(int k = 0; k < 5; ++k) {
                QTreeWidgetItem *p3 = addTreeChild(p2, unit[k], 2);
                for(int l = 0; l < 4; ++l) {
                    addTreeChild(p3, house[l], 3);
                }
            }
        }
    }
}

//初始化数据库
void TreeChoose::initDatabase() {
    QSqlDatabase qSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    database.setQSqlDatabase(qSqlDatabase);

}

QTreeWidgetItem* TreeChoose::addTreeRoot(QString name, int colNum) {
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);
    treeItem->setText(colNum, name);
    return treeItem;
}

 QTreeWidgetItem* TreeChoose::addTreeChild(QTreeWidgetItem *parent, QString name, int colNum) {
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(colNum, name);
    parent->addChild(treeItem);
    return treeItem;
}

