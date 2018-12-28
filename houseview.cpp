#include "houseview.h"
#include "ui_houseview.h"


Database HouseView::database;
QList<House> HouseView::houses;
int HouseView::row;
int HouseView::col;
int HouseView::rowSum;
int HouseView::colSum;

HouseView::HouseView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HouseView)
{
    ui->setupUi(this);


    initTable();
    initDateTime();
    setHouse();
    //setHouse();
    //setTimer();
}

HouseView::~HouseView()
{
    delete ui;
}

void HouseView::initDatabase() {
    QSqlDatabase qSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    database.setQSqlDatabase(qSqlDatabase);
}

void HouseView::initTable() {

    rowSum = 17;
    colSum = 5;

    ui->table->setRowCount(rowSum);
    ui->table->setColumnCount(colSum);

    for(int row = 0; row < rowSum; row++) {
        for(int col = 0; col < colSum; col++) {
            //门牌号
            QString houseNo;
            if(colSum<10) {
                houseNo = QString::number(rowSum-row+1)+"0"+QString::number(col+1);
            } else {
                houseNo = QString::number(rowSum-row+1)+QString::number(col+1);
            }

            //单元格
            QTableWidgetItem *item = new QTableWidgetItem(houseNo);

            //设置内容
            item->setTextAlignment(Qt::AlignCenter);

            //设置背景色
            QColor color(238, 240, 244, 255);
            item->setBackgroundColor(color);

            ui->table->setItem(row, col, item);
        }
    }
}

void HouseView::setHouse() {

    QDateTime start = ui->startDateTime->dateTime();
    QDateTime end = ui->endDateTime->dateTime();

    initDatabase();
    database.openConnect();
    houses = database.selectHouse(start, end);
    database.closeConnect();

    changeHouseStatus();

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

    for(int i = 0; i < rowSum; i++) {
        for(int j = 0; j < colSum; j++) {
            QTableWidgetItem *item = ui->table->takeItem(i, j);
            item->setBackgroundColor(glory);
            ui->table->setItem(i, j, item);
        }
    }

    for(int i = 0; i < houses.size(); i++) {
        QString houseNo = houses[i].house;
        if(houseNo.length() == 3) {

            QString rowString = houseNo.mid(0, 1);
            QString colString = houseNo.mid(1, 2);

            row = rowSum-rowString.toInt()+1;
            col = colString.toInt()-1;

            qDebug() << "HouseView::front:" << row;
            qDebug() << "HouseView::end" << col;

            QTableWidgetItem *item = ui->table->takeItem(row, col);
            item->setBackgroundColor(green);
            ui->table->setItem(row, col, item);

        } else if(houseNo.length() == 4){

            QString rowString = houseNo.mid(0, 2);
            QString colString = houseNo.mid(2, 2);

            row = rowSum-rowString.toInt()+1;
            col = colString.toInt()-1;

            qDebug() << "HouseView::front:" << row;
            qDebug() << "HouseView::end" << col;

            QTableWidgetItem *item = ui->table->takeItem(row, col);
            item->setBackgroundColor(green);
            ui->table->setItem(row, col, item);

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



    setHouse();


}
