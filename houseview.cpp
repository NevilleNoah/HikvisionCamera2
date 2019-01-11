#include "houseview.h"
#include "ui_houseview.h"


Database HouseView::database;
QList<House> HouseView::houses;
QList<RECORD> HouseView::records;
int HouseView::houseRow;
int HouseView::houseCol;
int HouseView::houseRowSum;
int HouseView::houseColSum;

int HouseView::recordRow;
int HouseView::recordCol;
int HouseView::recordRowSum;
int HouseView::recordColSum;

HouseView::HouseView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HouseView)
{
    ui->setupUi(this);

    initUI();

}

HouseView::~HouseView()
{
    delete ui;
}

void HouseView::initUI() {
    initRecordTable();
    initHouseTable();
    initDateTime();
    setHouseTable();
}

/**
 * @brief HouseView::initDatabase
 * 初始化数据库
 */
void HouseView::initDatabase() {
    QSqlDatabase qSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    database.setQSqlDatabase(qSqlDatabase);
}

/**
 * @brief HouseView::initInfoTable
 * 初始化特定门牌号用户的出入信息表
 */
void HouseView::initRecordTable() {
    //设置列宽
    ui->recordTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
}

void HouseView::initHouseTable() {

    houseRowSum = 17;
    houseColSum = 5;

    ui->houseTable->setRowCount(houseRowSum);
    ui->houseTable->setColumnCount(houseColSum);

    for(int row = 0; row < houseRowSum; row++) {
        for(int col = 0; col < houseColSum; col++) {
            //门牌号
            QString houseNo;
            if(houseColSum<10) {
                houseNo = QString::number(houseRowSum-row+1)+"0"+QString::number(col+1);
            } else {
                houseNo = QString::number(houseRowSum-row+1)+QString::number(col+1);
            }

            //单元格
            QTableWidgetItem *item = new QTableWidgetItem(houseNo);

            //设置内容
            item->setTextAlignment(Qt::AlignCenter);

            //设置背景色
            QColor color(238, 240, 244, 255);
            item->setBackgroundColor(color);

            ui->houseTable->setItem(row, col, item);
        }
    }
}

void HouseView::setHouseTable() {

    QDateTime start = ui->startDateTime->dateTime();
    QDateTime end = ui->endDateTime->dateTime();

    initDatabase();
    database.openConnect();
    houses = database.selectHouse(start, end);
    database.closeConnect();

    changeHouseStatus();

}

void HouseView::setRecordTable(QTableWidgetItem *item) {
    QDateTime start = ui->startDateTime->dateTime();
    QDateTime end = ui->endDateTime->dateTime();
    QString doorPlate = item->text();

    initDatabase();
    database.openConnect();
    records = database.selectByTimeDoorplate(start, end, doorPlate);
    database.closeConnect();

    recordRowSum = records.size();

    changeRecordTableData();
};

void HouseView::changeRecordTableData() {
    ui->recordTable->clearContents();
    ui->recordTable->setRowCount(recordRowSum);

    for(int i = 0; i < recordRowSum; i++) {
        QTableWidgetItem *timeItem = new QTableWidgetItem();
        QTableWidgetItem *applicantItem = new QTableWidgetItem();
        QTableWidgetItem *sfzNoItem = new QTableWidgetItem();

        timeItem->setText(records[i].timesamp.toString("yyyy-MM-dd ddd hh:mm"));
        applicantItem->setText(records[i].applicant);
        sfzNoItem->setText(records[i].idAvatar);

        ui->recordTable->setItem(i, 0, timeItem);
        ui->recordTable->setItem(i, 1, applicantItem);
        ui->recordTable->setItem(i, 2, sfzNoItem);

    }
}

void HouseView::setTimer() {
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(setHouseAsTimer()));
    timer->start(10000);
}

void HouseView::setHouseAsTimer() {
    qDebug("HouseView::timer exec........");

    initDatabase();
    database.openConnect();
    houses = database.selectHouseAsTimer();
    database.closeConnect();

    changeHouseStatus();

}

void HouseView::changeHouseStatus() {

    QColor green(129, 166, 96, 255);//绿色
    QColor glory(238, 240, 244, 255);//灰色

    for(int i = 0; i < houseRowSum; i++) {
        for(int j = 0; j < houseColSum; j++) {
            QTableWidgetItem *item = ui->houseTable->takeItem(i, j);
            item->setBackgroundColor(glory);
            ui->houseTable->setItem(i, j, item);
        }
    }

    for(int i = 0; i < houses.size(); i++) {
        QString houseNo = houses[i].house;
        if(houseNo.length() == 3) {

            QString rowString = houseNo.mid(0, 1);
            QString colString = houseNo.mid(1, 2);

            houseRow = houseRowSum-rowString.toInt()+1;
            houseCol = colString.toInt()-1;

            qDebug() << "HouseView::front:" << houseRow;
            qDebug() << "HouseView::end" << houseCol;

            QTableWidgetItem *item = ui->houseTable->takeItem(houseRow, houseCol);
            item->setBackgroundColor(green);
            ui->houseTable->setItem(houseRow, houseCol, item);

        } else if(houseNo.length() == 4){

            QString rowString = houseNo.mid(0, 2);
            QString colString = houseNo.mid(2, 2);

            houseRow = houseRowSum-rowString.toInt()+1;
            houseCol = colString.toInt()-1;

            qDebug() << "HouseView::front:" << houseRow;
            qDebug() << "HouseView::end" << houseCol;

            QTableWidgetItem *item = ui->houseTable->takeItem(houseRow, houseCol);
            item->setBackgroundColor(green);
            ui->houseTable->setItem(houseRow, houseCol, item);

        } else {

        }
    }
}

void HouseView::initDateTime() {
    QDate startDate = QDate::currentDate();
    QTime startTime;
    startTime.setHMS(0, 0, 0);
    ui->startDateTime->setDate(startDate);
    ui->startDateTime->setTime(startTime);

    QDate endDate = QDate::currentDate();
    QTime endTime;
    endTime.setHMS(23, 59, 59);
    ui->endDateTime->setDate(endDate);
    ui->endDateTime->setTime(endTime);

    //setEdStartTime(HistoryView::startDateTime);
    //setEdEndTime(HistoryView::endDateTime);
}


void HouseView::on_flush_clicked()
{

    setHouseTable();

}



void HouseView::on_houseTable_itemDoubleClicked(QTableWidgetItem *item)
{
    setRecordTable(item);
}
