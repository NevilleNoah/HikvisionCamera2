#include "settingsview.h"
#include "ui_settingsview.h"

SettingsView::SettingsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsView)
{
    ui->setupUi(this);
}


SettingsView::~SettingsView()
{
    delete ui;
}
/*****************************************线程*****************************************/
void SettingsView::runReadSettingsThread() {
    settingsThread = new SettingsThread(this);
    settingsThread->setStatus(SettingsThread::STATUS_READ);

    connect(settingsThread, SIGNAL(readedCameraSettings(QString, int, QString, QString)),
            this, SLOT(loadCameraSettings(QString, int, QString, QString)));
    connect(settingsThread, SIGNAL(readedDatabaseSettings(QString, int, QString, QString)),
            this, SLOT(loadDatabaseSettings(QString, int, QString, QString)));
    connect(settingsThread, SIGNAL(readedSettings()),
            this, SLOT(setBtnDetermineEnable()));
    //connect(settingsThread, SIGNAL(writedSettings()),
            //this, SLOT(setBtnDetermineEnable()));

    settingsThread->start();
}

void SettingsView::runWriteSettingsThread() {
    settingsThread = new SettingsThread(this);
    settingsThread->setStatus(SettingsThread::STATUS_WRITE);
    settingsThread->start();
}
/***************************************线程 END***************************************/

/****************************************更新Ui****************************************/
//更新Ui：加载摄像机配置
void SettingsView::loadCameraSettings(QString ip, int port, QString username, QString password) {
    ui->edCMIP->setText(ip);
    ui->edCMPort->setText(QString::number(port));
    ui->edCMUsername->setText(username);
    ui->edCMPassword->setText(password);
}

//更新Ui：加载数据库配置
void SettingsView::loadDatabaseSettings(QString ip, int port, QString username, QString password) {
    ui->edDBIP->setText(ip);
    ui->edDBPort->setText(QString::number(port));
    ui->edDBUsername->setText(username);
    ui->edDBPassword->setText(password);
}
//更新Ui：按键生效
void SettingsView::setBtnDetermineEnable() {
    ui->btnDetermine->setEnabled(true);
}
/**************************************更新Ui END**************************************/

/****************************************更新数据****************************************/
void SettingsView::changeCameraSettings() {

    SettingsThread::CMIp = ui->edCMIP->text();
    SettingsThread::CMPort = ui->edCMPort->text().toInt();
    SettingsThread::CMUsername = ui->edCMUsername->text();
    SettingsThread::CMPassword = ui->edCMPassword->text();

}

void SettingsView::changeDatabaseSettings() {

    SettingsThread::DBIp = ui->edDBIP->text();
    SettingsThread::DBPort = ui->edDBPort->text().toInt();
    SettingsThread::DBUsername = ui->edDBUsername->text();
    SettingsThread::DBPassword = ui->edDBPassword->text();

}

void SettingsView::on_btnDetermine_clicked()
{
    qDebug() << "SettingsView: on_btnDetermine_clicked exec";

    //ui->btnDetermine->setEnabled(false);

    changeCameraSettings();
    changeDatabaseSettings();

    settingsThread = new SettingsThread(this);
    settingsThread->setStatus(SettingsThread::STATUS_WRITE);
    settingsThread->run();

}
/*************************************更新数据 END**************************************/
