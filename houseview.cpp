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

QString HouseView::dirAvatar;
QString HouseView::dirCapture;
QString HouseView::dirFace;
QString HouseView::dirPicAvatar;
QString HouseView::dirPicCapture;
QString HouseView::dirPicFace;

HouseView::HouseView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HouseView)
{
    ui->setupUi(this);

    initUI();
    setTimer();

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

    ui->houseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->houseTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
    //database.openConnect();
    records = database.selectByTimeDoorplate(start, end, doorPlate);
   // database.closeConnect();

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
    connect(timer, SIGNAL(timeout()), this, SLOT(setHouseTable()));
    timer->start(10000);
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

            //qDebug() << "HouseView::front:" << houseRow;
            //qDebug() << "HouseView::end" << houseCol;

            QTableWidgetItem *item = ui->houseTable->takeItem(houseRow, houseCol);
            item->setBackgroundColor(green);
            ui->houseTable->setItem(houseRow, houseCol, item);

        } else if(houseNo.length() == 4){

            QString rowString = houseNo.mid(0, 2);
            QString colString = houseNo.mid(2, 2);

            houseRow = houseRowSum-rowString.toInt()+1;
            houseCol = colString.toInt()-1;

            //qDebug() << "HouseView::front:" << houseRow;
            //qDebug() << "HouseView::end" << houseCol;

            QTableWidgetItem *item = ui->houseTable->takeItem(houseRow, houseCol);
            item->setBackgroundColor(green);
            ui->houseTable->setItem(houseRow, houseCol, item);

        } else {

        }
    }
}

void HouseView::initDateTime() {
    ui->startDateTime->setDisplayFormat("yyyy-MM-dd ddd hh:mm");
    ui->endDateTime->setDisplayFormat("yyyy-MM-dd ddd hh:mm");

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


void HouseView::on_flush_clicked() {
    setHouseTable();
}

void HouseView::on_houseTable_itemDoubleClicked(QTableWidgetItem *item) {
    //双击之后清空显示的图片与个人信息
    setCapturePic(QImage(""));
    setAvatarPic(QImage(""));
    setFacePic(QImage(""));
    setPersonInfo("", "", "", "");

    setRecordTable(item);
}

void HouseView::setCapturePic(QImage imgCapture) {
    ui->picCapture->setPixmap(QPixmap::fromImage(imgCapture).scaled(ui->picCapture->size(),
                              Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void HouseView::setAvatarPic(QImage imgAvatar) {
    ui->picAvatar->setPixmap(QPixmap::fromImage(imgAvatar).scaled(ui->picAvatar->size(),
                             Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void HouseView::setFacePic(QImage imgFace) {
    ui->picFace->setPixmap(QPixmap::fromImage(imgFace).scaled(ui->picFace->size(),
                           Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void HouseView::getPicDir(RECORD record) {
    dirCapture = Config::getDirInfoCapture();
    dirAvatar = Config::getDirInfoAvatar();
    dirFace = Config::getDirInfoFace();
    dirPicCapture = dirCapture.append(record.idCapture+".jpg");
    dirPicFace = dirFace.append(record.idCapture+".jpg");
    dirPicAvatar = dirAvatar.append(record.idAvatar+".jpg");
}

void HouseView::setPersonInfo(QString applicant, QString similar,
                              QString idAvatar, QString familyRole) {
    ui->edName->setText(applicant);
    ui->edSimilarity->setText(similar);
    ui->edNo->setText(idAvatar);
    ui->edStatus->setText(familyRole);
}

void HouseView::on_recordTable_itemDoubleClicked(QTableWidgetItem *item)
{
    initDatabase();
    RECORD record = database.selectRecord(records[item->row()].timesamp,
                                            records[item->row()].applicant, records[item->row()].idAvatar);
    QString familyRole = database.selectFamilyRole(record.applicant, record.idAvatar);

    getPicDir(record);
    setCapturePic(QImage(dirPicCapture, "JPG"));
    setFacePic(QImage(dirPicFace, "JPG"));
    setAvatarPic(QImage(dirPicAvatar, "JPG"));
    setPersonInfo(record.applicant, QString::number(record.similar),
                  record.idAvatar, familyRole);
}

#include "excelutil.h"
#include <QFileDialog>
void HouseView::on_exportbtn_clicked()
{
    QDateTime start = ui->startDateTime->dateTime();
    QDateTime end = ui->endDateTime->dateTime();
    QString filename = QFileDialog::getSaveFileName(this, tr("Exceld导出路径"),  "", tr("*.xlsx;;")); //选择路径
    ExcelUtil *pWorker = new ExcelUtil();
    //connect(pWorker, SIGNAL(errorSignal(QString)), this, SLOT(onError(QString)));
    //connect(pWorker, SIGNAL(progressData(int, QString)), this, SLOT(onProgress(int, QString)));
    //connect(pWorker, SIGNAL(finish()), this, SLOT(onFinish()));
    pWorker->setPath(filename);
    pWorker->setStartTime(start);
    pWorker->setEndTime(end);
    QThread *thread = new QThread;
    pWorker->moveToThread(thread);
    //处理数据
    connect(thread, SIGNAL(started()), pWorker, SLOT(startTrans()), Qt::QueuedConnection);
    thread->start();
}
