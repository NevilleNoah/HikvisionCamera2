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


}

//初始化数据库
void TreeChoose::initDatabase() {
    QSqlDatabase qSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    database.setQSqlDatabase(qSqlDatabase);

}

