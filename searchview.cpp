#include "searchview.h"
#include "ui_searchview.h"
#include "database.h"

Database SearchView::database;
int SearchView::nowPage = 0;
int SearchView::pageNum = 0;
QList<ApplicantInfo> SearchView::applicantInfos;
QString SearchView::Name;
QString SearchView::idNumber = "";
int SearchView::totalRecordNum = 0;
const int SearchView::fieldNum = 4;

SearchView::SearchView(QWidget *parent) : QWidget(parent),
    ui(new Ui::SearchView) {
    ui->setupUi(this);
    initDatabase();
    initUI();
    initDataShow();
    operateType = Add;
}

SearchView::~SearchView()
{
    delete ui;
}

//初始化UI设置
void SearchView::initUI() {
    initTextNowPageUI();
    initTableUI();
    //changeview = new ChangeView;
    //searchView = new SearchView;
}

//初始化当前页码控件的UI
void SearchView::initTextNowPageUI() {
    //内容靠右侧显示
    ui->lineNowPage->setAlignment(Qt::AlignRight);
}

//初始化表格的UI
void SearchView::initTableUI() {
    //设置选择时选择一行
    ui->tableRecord->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置不留空
    ui->tableRecord->horizontalHeader()->setStretchLastSection(true);
    //设置列宽
    //ui->tableRecord->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    //设置表格行数
    ui->tableRecord->setRowCount(pageSize);
    //设置表头伸缩格式
    ui->tableRecord->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableRecord->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//初始化数据库
void SearchView::initDatabase() {
    QSqlDatabase qSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    database.setQSqlDatabase(qSqlDatabase);
    //database.openConnect();
}

//初始化数据显示
void SearchView::initDataShow() {
    on_btnCondition_clicked();
}

//获取用户输入的身份证编号
QString SearchView::getEdIdNumber() {
    return ui->idNumber->text();
}

//获取用户输入的姓名
QString SearchView::getEdName() {
    return ui->textName->text();
}


//设置当前页码控件的输入范围
void SearchView::setTextNowPageVal() {
    ui->lineNowPage->setValidator(new QIntValidator(1, pageNum, this));
}

//设置当前显示的页码
void SearchView::setTextNowPageNum(int nowPageNum) {
    ui->lineNowPage->setText(QString::number(nowPageNum));
}

//设置总页码的显示
void SearchView::setTextPageNumUI(int toltalPageNum) {
    ui->labPageNum->setText("/ "+QString::number(toltalPageNum));
}


//计算总页码
int SearchView::calPageNum() {
    qDebug() << "totalRecordNum: " << totalRecordNum;
    return pageNum = totalRecordNum%pageSize ? totalRecordNum/pageSize+1 : totalRecordNum/pageSize;
}

//根据页码获取数据
int SearchView::getRecordByPageNum(int startId) {
    initDatabase();
    database.openConnect();
    applicantInfos = database.selectById(startId,pageSize,getEdName(),getEdIdNumber(),"","",totalRecordNum);
    database.closeConnect();
    return applicantInfos.size();
}

//清除表格中的内容
void SearchView::clearTable() {
    ui->tableRecord->clearContents();
}

//点击上一页按钮
void SearchView::on_btnPrePage_3_clicked()
{
    //如果当前已经是第一页，那么点击按钮后无效果
    if(nowPage-1 <= 0)
        return;
    nowPage -= 1;
    //设置当前页码
    setTextNowPageNum(nowPage);
    //获取当前页面要显示的数据
    getRecordByPageNum(nowPage);
    emit showDataByCondition();
}

//点击下一页按钮
void SearchView::on_btnNextPage_3_clicked()
{
    //如果当前已经是最后一页，那么点击按钮无效果
    if(nowPage+1 > pageNum)
        return;
    //设置当前页码
    setTextNowPageNum(nowPage+=1);
    //获取当前页面要显示的数据
    getRecordByPageNum(nowPage);
    emit showDataByCondition();
}

//点击跳转按钮
void SearchView::on_btnJopToPage_clicked()
{
    //获取要跳转的页码
    QString turnPage = ui->lineNowPage->text();
    nowPage = turnPage.toInt();
    //获取当前页面要显示的数据
    getRecordByPageNum(nowPage);
    emit showDataByCondition();
}



//显示记录
void SearchView::showDataByCondition() {
    qDebug() << "SearchView: showByDateTimeRange exec";

    setTextPageNumUI(pageNum);
    clearTable();

    for(int i = 0; i < applicantInfos.size(); i++) {
        QTableWidgetItem *item[this->fieldNum];
        for(int j = 0; j < fieldNum; j++) {
            switch(j) {
            case 0: item[j] = new QTableWidgetItem(applicantInfos[i].applicant);
                break;
            case 1: item[j] = new QTableWidgetItem(applicantInfos[i].sfzno);
                break;
            case 2: item[j] = new QTableWidgetItem(applicantInfos[i].contact);
                break;
            case 3: item[j] = new QTableWidgetItem(applicantInfos[i].status);
                break;
            }
            //设置只读
            item[j]->setFlags(item[j]->flags() ^ Qt::ItemIsEditable);
            //设置对齐方式
            item[j]->setTextAlignment(Qt::AlignCenter);
            //设置单元格内容
            ui->tableRecord->setItem(i, j, item[j]);
        }
    }
}




void SearchView::on_Edit_clicked()
{
    qDebug()<<"onEdit";
    operateType = Edit;
    changeview = new ChangeView;
    //    changeview->show();

    QStringList rowData ;
    rowData = this->getCurrentRowData();
    if(rowData.isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("提示") , QStringLiteral("请选中需要编辑的数据!"));
        return ;
    }
    else
    {
        changeview->setEditData(rowData);
    }
    connect(this, SIGNAL(signaloperateInfo(OperateType)) , changeview ,SLOT(Exec(OperateType)));
    emit signaloperateInfo(operateType);
    changeview->activateWindow();
    changeview->setWindowTitle(QStringLiteral("修改:信息"));
    changeview->exec();

    //changeview->activateWindow();
    //changeview->setWindowTitle(QStringLiteral("修改: 学生信息"));
}


QStringList SearchView::getCurrentRowData()
{
    QStringList rowData;
    int row = ui->tableRecord->currentRow();
    if(row != -1)
    {
        rowData << ui->tableRecord->item(row , 0)->text();
        rowData << ui->tableRecord->item(row , 1)->text();
        rowData << ui->tableRecord->item(row , 2)->text();
        rowData << ui->tableRecord->item(row , 3)->text();
    }
    return rowData;
}

//点击确认按钮
void SearchView::on_btnCondition_clicked()
{
    //获取用户输入的身份证编号
    idNumber = getEdIdNumber();
    qDebug() << "idNumber' length:" << idNumber.length();

    //获取用户输入的姓名
    Name = getEdName();
    qDebug() << "textName' length:" << Name.length();


    //获取当前页面要显示的数据
    int recordNum = getRecordByPageNum(1);
    //计算总页数
    calPageNum();
    qDebug() << "calPageNum: " << pageNum;
    //pageNum = calPageNum();

    //显示总页数
    setTextPageNumUI(pageNum);
    //显示当前页码
    setTextNowPageNum(nowPage=recordNum?1:0);
    //设置页码的输入范围
    setTextNowPageVal();
    emit showDataByCondition();

}

//单击表格内容
void SearchView::on_tableRecord_itemClicked(QTableWidgetItem *item)
{

}

void SearchView::on_AddItem_clicked()
{
    qDebug()<<"onEdit";
    operateType = Add;
    changeview = new ChangeView;

    connect(this, SIGNAL(signaloperateInfo(OperateType)) , changeview ,SLOT(Exec(OperateType)));
    emit signaloperateInfo(operateType);
    changeview->activateWindow();
    changeview->setWindowTitle(QStringLiteral("添加:信息"));
    changeview->exec();
}

void SearchView::on_DelItem_clicked()
{

    qDebug()<<"onDel";
    QStringList rowData ;
    rowData = this->getCurrentRowData();
    if(rowData.isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("提示") , QStringLiteral("请选中需要删除的数据!"));
        return ;
    }
    else
    {
        QDateTime time;
        time = QDateTime::fromString(rowData.at(0), "yyyy-MM-dd hh:mm:ss");
        initDatabase();
        database.searchRecord(rowData.at(0),rowData.at(1));
        database.delRecord();
        on_btnJopToPage_clicked();
    }
}
