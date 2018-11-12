#include "historyview.h"
#include "ui_historyview.h"
#include "database.h"

Database HistoryView::db;
QList<RECORD> HistoryView::records;
QDateTime HistoryView::startDateTime;
QDateTime HistoryView::endDateTime;
int HistoryView::nowPage = 1;

HistoryView::HistoryView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryView)
{
    ui->setupUi(this);

    initTable();
    initTimeEdit();
    initData();

}

//初始化从数据库获取的数据
void HistoryView::initData() {

    QSqlDatabase qSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    db.setQSqlDatabase(qSqlDatabase);
    on_btnSearchByTime_clicked();
    /*db.openConnect();
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
   */
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

//初始化时间编辑器
void HistoryView::initTimeEdit() {
    QDate startDate = QDate::currentDate();
    QTime startTime;
    startTime.setHMS(0, 0, 0);
    startDateTime.setDate(startDate);
    startDateTime.setTime(startTime);

    QDate endDate = QDate::currentDate();
    QTime endTime;
    endTime.setHMS(23, 59, 59);
    endDateTime.setDate(endDate);
    endDateTime.setTime(endTime);

    ui->edStartTime->setDisplayFormat("yyyy-MM-dd ddd hh:mm");
    ui->edEndTime->setDisplayFormat("yyyy-MM-dd ddd hh:mm");

    ui->edStartTime->setDateTime(startDateTime);
    ui->edEndTime->setDateTime(endDateTime);

}

HistoryView::~HistoryView()
{
    delete ui;
}

void HistoryView::showByDateTimeRange() {
    qDebug() << "HistoryView: showByDateTimeRange exec";


    ui->recordTable->clearContents();
    ui->recordTable->setRowCount(pageSize);

    for(int i = 0, row = 0; i<records.size(); i++, row++) {
        QTableWidgetItem *timeItem, *nameItem, *sexItem, *idItem;

        timeItem = new QTableWidgetItem(records[i].timesamp.toString());

        if(!records[i].isStranger) {
            nameItem = new QTableWidgetItem(records[i].nameValue);
        } else {
            nameItem = new QTableWidgetItem(QString::fromLocal8Bit("陌生人"));
        }

        if(records[i].sex!=NULL && records[i].sex.length()>0){
            sexItem = new QTableWidgetItem(records[i].sex);
        } else {
             sexItem = new QTableWidgetItem(QString::fromLocal8Bit("未知"));
        }

        if(records[i].idNo!=NULL && records[i].idNo.length()>0) {
            idItem = new QTableWidgetItem(records[i].idNo);
        } else {
            idItem = new QTableWidgetItem(QString::fromLocal8Bit("未知"));
        }

        //设置只读
        timeItem->setFlags(timeItem->flags() ^ Qt::ItemIsEditable);
        nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
        sexItem->setFlags(sexItem->flags() ^ Qt::ItemIsEditable);
        idItem->setFlags(idItem->flags() ^ Qt::ItemIsEditable);

        //设置对齐方式
        timeItem->setTextAlignment(Qt::AlignCenter);
        nameItem->setTextAlignment(Qt::AlignCenter);
        sexItem->setTextAlignment(Qt::AlignCenter);
        idItem->setTextAlignment(Qt::AlignCenter);

        ui->recordTable->setItem(i, 0, timeItem);
        ui->recordTable->setItem(i, 1, nameItem);
        ui->recordTable->setItem(i, 2, sexItem);
        ui->recordTable->setItem(i, 3, idItem);
    }
}

/**************************zjb*******************************/
void HistoryView::on_btnPrePage_clicked()
{
    nowPage = nowPage-1 > 0 ? nowPage-1 : 1;      //如果当前已经是第一页，那么点击按钮后页码不变
    db.openConnect();
    records = db.selectByDateTimeRange(startDateTime, endDateTime, nowPage, pageSize, totalRecordNum);
    db.closeConnect();

    emit showByDateTimeRange();

}

void HistoryView::on_btnNextPage_clicked()
{
    nowPage = nowPage+1 > pageNum ? pageNum : nowPage+1;      //如果当前已经是第一页，那么点击按钮后页码不变
    db.openConnect();
    records = db.selectByDateTimeRange(startDateTime, endDateTime, nowPage, pageSize, totalRecordNum);
    db.closeConnect();
    nowPage = nowPage+1 > pageNum ? pageNum : nowPage+1;      //如果当前已经是第一页，那么点击按钮后页码不变
    emit showByDateTimeRange();
}

void HistoryView::on_btnSearchByTime_clicked()
{
    db.openConnect();
    startDateTime = ui->edStartTime->dateTime();
    endDateTime = ui->edEndTime->dateTime();
    records = db.selectByDateTimeRange(startDateTime, endDateTime, 1, pageSize, totalRecordNum);
    db.closeConnect();

    pageNum = totalRecordNum / pageSize;//总的页码
    if(totalRecordNum % pageSize)
        pageNum += 1;
    qDebug() << "totalRecordNum:" << totalRecordNum;
    qDebug() << "pageNum:" << pageNum;
    emit showByDateTimeRange();
}
/**************************zjb END***************************/
