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

    initDatabase();
    initTable();
    setHouse();
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

    rowSum = 18;
    colSum = 5;

    ui->table->setRowCount(rowSum);
    ui->table->setColumnCount(colSum);

    for(int row = 0; row < rowSum; row++) {
        for(int col = 0; col < colSum; col++) {
            //门牌号
            QString houseNo;
            if(colSum<10) {
                houseNo = QString::number(rowSum-row)+"0"+QString::number(col+1);
            } else {
                houseNo = QString::number(rowSum-row)+QString::number(col+1);
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

    QDateTime start = QDateTime();
    QDateTime end = QDateTime();

    database.openConnect();
    houses = database.selectHouse(start, end);
    database.closeConnect();

    changeHouseStatus();

}

void HouseView::changeHouseStatus() {

    QColor color(129, 166, 96, 255);//绿色

    for(int i = 0; i < houses.size(); i++) {
        QString houseNo = houses[i].house;
        if(houseNo.length() == 3) {

            QString rowString = houseNo.mid(0, 1);
            QString colString = houseNo.mid(1, 2);

            row = rowSum-rowString.toInt();
            col = colString.toInt()-1;

            qDebug() << "HouseView::front:" << row;
            qDebug() << "HouseView::end" << col;

            QTableWidgetItem *item = ui->table->takeItem(row, col);
            item->setBackgroundColor(color);
            ui->table->setItem(row, col, item);

        } else if(houseNo.length() == 4){

            QString rowString = houseNo.mid(0, 2);
            QString colString = houseNo.mid(2, 2);

            row = rowSum-rowString.toInt();
            col = colString.toInt()-1;

            qDebug() << "HouseView::front:" << row;
            qDebug() << "HouseView::end" << col;

            QTableWidgetItem *item = ui->table->takeItem(row, col);
            item->setBackgroundColor(color);
            ui->table->setItem(row, col, item);

        } else {

        }
    }
}

