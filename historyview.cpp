#include "historyview.h"
#include "ui_historyview.h"
#include "database.h"

Database HistoryView::db;
QList<RECORD> HistoryView::records;
QDateTime HistoryView::startDateTime;
QDateTime HistoryView::endDateTime;
int HistoryView::pageNum = 0;
int HistoryView::totalRecordNum = 0;
int HistoryView::nowPage = 0;
const int HistoryView::fieldNum = 4;

HistoryView::HistoryView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryView) {

    ui->setupUi(this);
    initDataBase();
    initTimeEdit();
    initUI();
    initDataShow();

}

HistoryView::~HistoryView()
{
    delete ui;
}

//初始化UI设置
void HistoryView::initUI() {
    initEdStartTimeUI();
    initEdEndTimeUI();
    initTextNowPageUI();
    initTableUI();
    initCheckBoxUI();
}

//设置选项控件UI
void HistoryView::initCheckBoxUI() {
    ui->chkStranger->setCheckState(Qt::Checked);
    ui->chkNoStranger->setCheckState(Qt::Checked);
    ui->chkMale->setCheckState(Qt::Checked);
    ui->chkFemale->setCheckState(Qt::Checked);
}

//开始时间控件的UI设置
void HistoryView::initEdStartTimeUI() {
     ui->edStartTime->setDisplayFormat("yyyy-MM-dd ddd hh:mm");
}

//结束时间控件的UI设置
void HistoryView::initEdEndTimeUI() {
    ui->edEndTime->setDisplayFormat("yyyy-MM-dd ddd hh:mm");
}

//设置当前页码控件的UI
void HistoryView::initTextNowPageUI() {
    //内容靠右侧显示
    ui->textNowPage->setAlignment(Qt::AlignRight);
}

//设置当前页码控件的输入范围
void HistoryView::setTextNowPageVal() {
    ui->textNowPage->setValidator(new QIntValidator(1, pageNum, this));
}

//设置表格的UI
void HistoryView::initTableUI() {
    //设置选择时选择一行
    ui->recordTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置不留空
    ui->recordTable->horizontalHeader()->setStretchLastSection(true);
    //设置列宽
    ui->recordTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    //设置表格行数
    ui->recordTable->setRowCount(pageSize);
}

//初始化数据库
void HistoryView::initDataBase() {
    QSqlDatabase qSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    db.setQSqlDatabase(qSqlDatabase);
}

//初始化数据显示
void HistoryView::initDataShow() {
    on_btnSearchByTime_clicked();
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

    setEdStartTime(HistoryView::startDateTime);
    setEdEndTime(HistoryView::endDateTime);
}

//设置开始时间控件的内容
void HistoryView::setEdStartTime(QDateTime startDateTime) {
    ui->edStartTime->setDateTime(startDateTime);
}

//设置结束时间控件的内容
void HistoryView::setEdEndTime(QDateTime endDateTime) {
     ui->edEndTime->setDateTime(endDateTime);
}

//获取开始时间控件的内容
QDateTime HistoryView::getEdStartTime() {
    return ui->edStartTime->dateTime();
}

//获取结束时间控件的内容
QDateTime HistoryView::getEdEndTime() {
    return ui->edEndTime->dateTime();
}

//设置当前显示的页码
void HistoryView::setTextNowPageNum(int nowPageNum) {
    ui->textNowPage->setText(QString::number(nowPageNum));
}

//设置总页码的显示
void HistoryView::setTextPageNumUI(int toltalPageNum) {
    ui->textPageNum->setText("/ "+QString::number(toltalPageNum));
}

//计算总页码
int HistoryView::calPageNum() {
    return totalRecordNum%pageSize ? totalRecordNum/pageSize+1 : totalRecordNum/pageSize;
}

//根据页码获取数据
int HistoryView::getRecordByDateTimeRange(int startId) {
    db.openConnect();
    records = db.selectByDateTimeRange(startDateTime, endDateTime, startId, pageSize, totalRecordNum);
    db.closeConnect();
    return records.size();
}

//清除表格中的内容
void HistoryView::clearTable() {
    ui->recordTable->clearContents();
}

void HistoryView::on_btnPrePage_clicked()
{
    //如果当前已经是第一页，那么点击按钮后无效果
    if(nowPage-1 <= 0)
        return;
    nowPage -= 1;
    //设置当前页码
    setTextNowPageNum(nowPage);
    //获取当前页面要显示的数据
    getRecordByDateTimeRange(nowPage);
    emit showByDateTimeRange();
}

void HistoryView::on_btnNextPage_clicked()
{
    //如果当前已经是最后一页，那么点击按钮无效果
    if(nowPage+1 > pageNum)
        return;
    //设置当前页码
    setTextNowPageNum(nowPage+=1);
    //获取当前页面要显示的数据
    getRecordByDateTimeRange(nowPage);
    emit showByDateTimeRange();
}

void HistoryView::on_btnJumpPage_clicked()
{
    //获取要跳转的页码
    QString turnPage = ui->textNowPage->text();
    nowPage = turnPage.toInt();
    //获取当前页面要显示的数据
    getRecordByDateTimeRange(nowPage);
    emit showByDateTimeRange();
}

void HistoryView::on_btnSearchByTime_clicked()
{
    //获取开始时间与结束时间
    startDateTime = getEdStartTime();
    endDateTime = getEdEndTime();
    //获取当前页面要显示的数据
    int recordNum = getRecordByDateTimeRange(1);
    //计算总页数
    pageNum = calPageNum();
    //显示总页数
    setTextPageNumUI(pageNum);
    //显示当前页码
    setTextNowPageNum(nowPage=recordNum?1:0);
    //设置页码的输入范围
    setTextNowPageVal();
    emit showByDateTimeRange();
}

void HistoryView::showByDateTimeRange() {
    qDebug() << "HistoryView: showByDateTimeRange exec";

    setTextPageNumUI(pageNum);
    clearTable();

    for(int i = 0; i < records.size(); i++) {
        QTableWidgetItem *item[this->fieldNum];
        for(int j = 0; j < fieldNum; j++) {
            switch(j) {
            case 0: item[j] = new QTableWidgetItem(records[i].timesamp.toString("yyyy-MM-dd ddd hh:mm"));
                break;
            case 1: item[j] = !records[i].isStranger ? new QTableWidgetItem(records[i].nameValue)
                                                     : new QTableWidgetItem(QString::fromLocal8Bit("陌生人"));
                break;
            case 2: item[j] = records[i].sex.length()>0 ? new QTableWidgetItem(records[i].sex)
                                                        : new QTableWidgetItem(QString::fromLocal8Bit("未知"));
                break;
            case 3: item[j] = records[i].idNo.length()>0 ? new QTableWidgetItem(records[i].idNo)
                                                         : new QTableWidgetItem(QString::fromLocal8Bit("未知"));
                break;
            }
            //设置只读
            item[j]->setFlags(item[j]->flags() ^ Qt::ItemIsEditable);
            //设置对齐方式
            item[j]->setTextAlignment(Qt::AlignCenter);
            //设置单元格内容
            ui->recordTable->setItem(i, j, item[j]);
        }
    }
}

//双击表格内容
void HistoryView::on_recordTable_itemDoubleClicked(QTableWidgetItem *item)
{
    QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);

    //人脸图
    QString avatarDir = config->value("/Dir/dirAvatar").toString();
    avatarDir.append(records[item->row()].idAvatar);
    avatarDir.append(".jpg");
    qDebug() << "avatarDir is "<<avatarDir;

    QImage avatarImg(avatarDir, "JPG");
    QPixmap avatarPix = QPixmap::fromImage(avatarImg);

    ui->picAvatar->setPixmap(avatarPix);

    //抓拍图
    QString captureDir = config->value("/Dir/dirCapture").toString();
    captureDir.append(records[item->row()].idCapture);
    captureDir.append(".jpg");
    qDebug() << "captureDir is "<<captureDir;

    QImage captureImg(captureDir, "JPG");
    QPixmap capturePix = QPixmap::fromImage(captureImg);

    ui->picCapture->setPixmap(capturePix);
}
