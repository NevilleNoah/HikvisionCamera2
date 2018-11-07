#include "historyview.h"
#include "ui_historyview.h"
#include "database.h"

Database HistoryView::db;
QList<RECORD> HistoryView::records;
QDateTime HistoryView::startDateTime;
QDateTime HistoryView::endDateTime;

HistoryView::HistoryView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryView)
{
    ui->setupUi(this);

    initTable();
    allData();

}

//初始化从数据库获取的数据
void HistoryView::allData() {

    QSqlDatabase qSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    db.setQSqlDatabase(qSqlDatabase);

    db.openConnect();
    records = db.selectRecord();
    db.closeConnect();

    ui->recordTable->setRowCount(records.size());


    for(int i = 0; i < records.size(); i++) {
        //设置单元格内容
        QTableWidgetItem* timesampItem;
        QTableWidgetItem* nameItem;
        QTableWidgetItem* sexItem;
        QTableWidgetItem* idNoItem;

        timesampItem = new QTableWidgetItem(records[i].timesamp.toString("yyyy-MM-dd hh:mm:ss"));
        if(!records[i].isStranger) {
            nameItem = new QTableWidgetItem(records[i].nameValue);
        } else {
            nameItem = new QTableWidgetItem(QString::fromLocal8Bit("陌生人"));
        }
        sexItem = new QTableWidgetItem(records[i].sex);
        if(records[i].idNo!=NULL && records[i].idNo.length()>0) {
            idNoItem = new QTableWidgetItem(records[i].idNo);
        } else {
            idNoItem = new QTableWidgetItem(QString::fromLocal8Bit("未知"));
        }
        //设置只读
        timesampItem->setFlags(timesampItem->flags() ^ Qt::ItemIsEditable);
        nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
        sexItem->setFlags(sexItem->flags() ^ Qt::ItemIsEditable);
        idNoItem->setFlags(idNoItem->flags() ^ Qt::ItemIsEditable);

        //设置对齐方式
        timesampItem->setTextAlignment(Qt::AlignCenter);
        nameItem->setTextAlignment(Qt::AlignCenter);
        sexItem->setTextAlignment(Qt::AlignCenter);
        idNoItem->setTextAlignment(Qt::AlignCenter);



        ui->recordTable->setItem(i, 0, timesampItem);
        ui->recordTable->setItem(i, 1, nameItem);
        ui->recordTable->setItem(i, 2, sexItem);
        ui->recordTable->setItem(i, 3, idNoItem);


    }
}

//初始化表格的基本属性
void HistoryView::initTable() {
    //设置选择时选择一行
    ui->recordTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置不留空
    ui->recordTable->horizontalHeader()->setStretchLastSection(true);
    //设置列宽
    ui->recordTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);


}

HistoryView::~HistoryView()
{
    delete ui;
}

void HistoryView::on_pushButton_clicked()
{
    db.openConnect();
    records = db.selectByDateTimeRange(startDateTime, endDateTime);
    db.closeConnect();
    emit showByDateTimeRange();
}

void HistoryView::showByDateTimeRange() {
    qDebug() << "HistoryView: showByDateTimeRange exec";

    ui->recordTable->clear();

    for(int i = 0; i<records.size();i++) {
        qDebug() << records[i].nameValue;
    }
}
