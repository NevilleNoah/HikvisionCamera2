#include "treechoose.h"
#include "ui_treechoose.h"
#include "database.h"
Database TreeChoose::database;
QList<HouseInfo> TreeChoose::houseInfos;

const int fieldNum = 4;

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
    houseInfos = database.searchHouse("update");

    QTreeWidgetItem *p1 = addTreeRoot(houseInfos[0].community, 0);
    QTreeWidgetItem *p2 = addTreeChild(p1, houseInfos[0].building, 1);
    QTreeWidgetItem *p3 = addTreeChild(p2, houseInfos[0].unit, 2);
    addTreeChild(p3, houseInfos[0].house, 3);

    QString communitytemp=houseInfos[0].community;
    QString buildingtemp=houseInfos[0].building;
    QString unittemp=houseInfos[0].unit;
    QString housetemp=houseInfos[0].house;

    for(int i = 1; i < houseInfos.size(); ++i) {
        if(houseInfos[i].community==communitytemp){
        }else{
            QTreeWidgetItem *p1 = addTreeRoot(houseInfos[i].community, 0);
            communitytemp=houseInfos[i].community;
        }
        if(houseInfos[i].building==buildingtemp){
        }else{
            QTreeWidgetItem *p2 = addTreeChild(p1, houseInfos[i].building, 1);
            buildingtemp=houseInfos[i].building;
        }

        if(houseInfos[i].unit==unittemp){
        }else{
            QTreeWidgetItem *p3 = addTreeChild(p2, houseInfos[i].unit, 2);
            unittemp=houseInfos[i].unit;
        }

        if(houseInfos[i].house==housetemp){
        }else{
            addTreeChild(p3, houseInfos[i].house, 3);
            housetemp=houseInfos[i].house;
        }
    }
}

void searchTree(QString root, QTreeWidgetItem *item){

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


void TreeChoose::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(column==fieldNum-1){
        QList<QString> array;
        int temp=column;
        while(item != nullptr){
            array.append(item->text(temp));
            item = item->parent() ;
            temp--;
        }
        int i = array.size()-1;
        houseInfo.community=array. at(i--);
        houseInfo.building=array. at(i--);
        houseInfo.unit=array. at(i--);
        houseInfo.house=array. at(i);
        emit signalhouseInfo(houseInfo);
        close();
    }else{

    }

}
