#include "changeview.h"
#include "ui_changeview.h"
#include "searchView.h"
#include <QDateTime>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QFileDialog>

QList<HouseInfo> ChangeView::houseInfos;
Database ChangeView::database;
bool searchsign;
HouseInfo houseInfotemp;
const int fieldNum = 4;

ChangeView::ChangeView(QWidget *parent) : QDialog(parent),ui(new Ui::ChangeView)
{
    ui->setupUi(this);
    QDateTime curTime;
    curTime.currentDateTime().fromString("yyyy-MM-dd hh:mm:ss");
    ui->DateEdit->setDateTime(curTime);
}

ChangeView::~ChangeView()
{
    delete ui;
}

void ChangeView::Exec(OperateType operateType)
{
    operate=operateType;
    if(operate == Add)
    {
    }
    else if(operate == Edit)
    {
        searchsign=database.searchRecord(ui->NameEdit->text(), ui->IdNumEdit->text());
    }else {
    }
}

void ChangeView::setEditData(QStringList rowData)
{
    QDateTime time;
    time = QDateTime::fromString(rowData.at(0), "yyyy-MM-dd hh:mm:ss");
    ui->DateEdit->setDateTime(time);
    ui->NameEdit->setText(rowData.at(0));
    ui->IdNumEdit->setText(rowData.at(1));
    ui->PhoneEdit->setText(rowData.at(2));
    ui->IdentityEdit->setText(rowData.at(3));
    searchsign=database.searchRecord(ui->NameEdit->text(), ui->IdNumEdit->text());
    houseInfos = database.searchHouse("search");
    QString house;
    houseInfotemp=houseInfos[0];
    qDebug() << "setEditData "  ;
    for(int i = 0; i < fieldNum; i++) {
        switch(i) {
        case 0:
            qDebug() << "case "<<i;
            house+=houseInfos[0].community+QStringLiteral("小区");
            break;
        case 1:
            qDebug() << "case "<<i;
            house+=houseInfos[0].building+QStringLiteral("幢");
            break;
        case 2:
            qDebug() << "case "<<i;
            house+=houseInfos[0].unit+QStringLiteral("单元");
            break;
        case 3:
            qDebug() << "case "<<i;
            house+=houseInfos[0].house+QStringLiteral("房间");
            break;
        }
    }
    ui->HouseNumEdit->setText(house);
}

void ChangeView::clearData()
{
    ui->NameEdit->clear();
    ui->IdNumEdit->clear();
    ui->PhoneEdit->clear();
    ui->IdentityEdit->clear();
}

void ChangeView::closeEvent(QCloseEvent *event)
{
    clearData();
}

void ChangeView::on_buttonBox_accepted()

{
    database.setQSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
    int similar=ui->SimilarEdit->text().toInt();
    bool sign;
    if(operate == Add)
    {
        sign=database.addApplicant(ui->NameEdit->text(), ui->IdNumEdit->text(), ui->PhoneEdit->text(),ui->IdentityEdit->text());
        if(sign==true){
            qDebug() << "asdugasiuashgdiuadsasdasd: " << ui->IdNumEdit->text();
            insert2SuperBrain(); //添加成功 住户信息上传至超脑
            QMessageBox::information(this ,QStringLiteral("提示") , QStringLiteral("添加成功!"));
        }else {
            QMessageBox::information(this ,QStringLiteral("提示") , QStringLiteral("添加失败!"));
        }

        database.updatehouseInfo(houseInfotemp);
        close();
    }
    else if(operate == Edit)
    {
        database.setQSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
        sign=database.editApplicant(ui->NameEdit->text(), ui->IdNumEdit->text(), ui->PhoneEdit->text(),ui->IdentityEdit->text());
        if(sign==true){
            QMessageBox::information(this ,QStringLiteral("提示") , QStringLiteral("成功!"));
        }else {
            QMessageBox::information(this ,QStringLiteral("提示") , QStringLiteral("失败!"));
        }

        database.updatehouseInfo(houseInfotemp);
        close();
    }
}

void ChangeView::on_buttonBox_rejected()
{
    close();
}

void ChangeView::on_toolButton_clicked()
{
    //获取文件名和路径，同时加入筛选功能
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QStringLiteral("选择图像"),//对话框名称
                                                    ".",//默认打开文件位置“.”文件目录"/"根目录
                                                    QStringLiteral("图片格式(*.jpg *.png *.bmp)"));//筛选器
    // 使用lebel显示之前获取的文件名和路径
    ui->lineEdit->setText(fileName);
    QPixmap pix(fileName);
    ui->imagelab->setPixmap(pix);

}

void ChangeView::on_toolButton_2_clicked()
{
    treechoose = new TreeChoose;
    connect(treechoose, SIGNAL(signalhouseInfo(HouseInfo)) , this ,SLOT(Refresh(HouseInfo)));
    treechoose->activateWindow();
    treechoose->setWindowTitle(QStringLiteral("选择房间"));
    treechoose->exec();
}

void ChangeView::Refresh(HouseInfo houseInfo)
{
    houseInfotemp=houseInfo;
    qDebug() << "refresh ";
    QString house;
    for(int i = 0; i < fieldNum; i++) {
        switch(i) {
        case 0:
            qDebug() << "case "<<i;
            house+=houseInfo.community+QStringLiteral("小区");
            break;
        case 1:
            qDebug() << "case "<<i;
            house+=houseInfo.building+QStringLiteral("幢");
            break;
        case 2:
            qDebug() << "case "<<i;
            house+=houseInfo.unit+QStringLiteral("单元");
            break;
        case 3:
            qDebug() << "case "<<i;
            house+=houseInfo.house+QStringLiteral("房间");
            break;
        }
        qDebug() << "refresh house "<<house;
    }
    ui->HouseNumEdit->setText(house);
}

#include "previewview.h"
void ChangeView::insert2SuperBrain() {
    QString fileName = ui->lineEdit->text();
    QString idNumber = ui->IdNumEdit->text();
    PreviewView::uploadStrangerFacePic(idNumber, fileName);
}
