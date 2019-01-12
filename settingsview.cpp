#include "settingsview.h"

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
    connect(settingsThread, SIGNAL(readedCameraSettings()),
            this, SLOT(loadCameraSettings()));
    connect(settingsThread, SIGNAL(readedDatabaseSettings()),
            this, SLOT(loadDatabaseSettings()));
    connect(settingsThread, SIGNAL(readedPicDirSettings()),
            this, SLOT(loadPicDirSettings()));
    connect(settingsThread, SIGNAL(readedSettings()),
            this, SLOT(setBtnDetermineEnable()));
    settingsThread->start();
}

void SettingsView::runWriteSettingsThread() {
    settingsThread = new SettingsThread(this);
    settingsThread->setStatus(SettingsThread::STATUS_WRITE);
    settingsThread->start();
}
/***************************************线程 END***************************************/

/****************************************更新Ui****************************************/
//更新Ui：加载摄像机配
void SettingsView::loadCameraSettings() {
    int *channels = Config::getCameraInfoChannel();
    for(int i = 0; i < 4; i++) {
        qDebug() << "loadCameraSettings(settingsview.cpp):" << channels[i];
    }
    ui->edCMIP->setText(Config::getCameraInfoIP());
    ui->edCMPort->setText(QString::number(Config::getCameraInfoPort()));
    ui->edCMUsername->setText(Config::getCameraInfoUserName());
    ui->edCMPassword->setText(Config::getCameraInfoPassWord());
    ui->edCMChannel1->setText(QString::number(channels[0]));
    ui->edCMChannel2->setText(QString::number(channels[1]));
    ui->edCMChannel3->setText(QString::number(channels[2]));
    ui->edCMChannel4->setText(QString::number(channels[3]));
}


//更新Ui：加载数据库配置
void SettingsView::loadDatabaseSettings() {
    qDebug() << "settingsview DBInfo: " << Config::getDataBaseInfoIP();
    ui->edDBIP->setText(Config::getDataBaseInfoIP());
    ui->edDBPort->setText(QString::number(Config::getDataBaseInfoPort()));
    ui->edDBModel->setText(Config::getDataBaseInfoModel());
    ui->edDBUsername->setText(Config::getDataBaseInfoUserName());
    ui->edDBPassword->setText(Config::getDataBaseInfoPassWord());
}

//更新Ui：加载路径配置
void SettingsView::loadPicDirSettings() {
    ui->edDirCapture->setText(Config::getDirInfoCapture());
    ui->edDirAvatar->setText(Config::getDirInfoAvatar());
    ui->edDirFace->setText(Config::getDirInfoFace());
}
//更新Ui：按键生效
void SettingsView::setBtnDetermineEnable() {
    ui->btnDetermine->setEnabled(true);
}
/**************************************更新Ui END**************************************/

/****************************************更新数据****************************************/
void SettingsView::changeCameraSettings() {
    int *channel = new int[4];
    channel[0] = ui->edCMChannel1->text().toInt();
    channel[1] = ui->edCMChannel2->text().toInt();
    channel[2] = ui->edCMChannel3->text().toInt();
    channel[3] = ui->edCMChannel4->text().toInt();
    for(int i = 0; i < 4; i++) {
        qDebug() << "changeCameraSettings(settingsview.cpp):" << channel[i];
    }
    Config::setCameraInfoChannel(channel);
    Config::setCameraInfoIP(ui->edCMIP->text());
    Config::setCameraInfoPort(ui->edCMPort->text().toInt());
    Config::setCameraInfoUserName(ui->edCMUsername->text());
    Config::setCameraInfoPassWord(ui->edCMPassword->text());

    qDebug() << "settingsview cameraInfo.ip: " << Config::getCameraInfoIP();
}

void SettingsView::changeDatabaseSettings() {
    Config::setDataBaseInfoIP(ui->edDBIP->text());
    Config::setDataBaseInfoPort(ui->edDBPort->text().toInt());
    Config::setDataBaseInfoModel(ui->edDBModel->text());
    Config::setDataBaseInfoUserName(ui->edDBUsername->text());
    Config::setDataBaseInfoPassWord(ui->edDBPassword->text());
}

void SettingsView::changePicDirSettings() {
    Config::setDirInfoAvatar(ui->edDirAvatar->text());
    Config::setDirInfoCapture(ui->edDirCapture->text());
    Config::setDirInfoFace(ui->edDirFace->text());
}

void SettingsView::on_btnDetermine_clicked()
{
    qDebug() << "SettingsView: on_btnDetermine_clicked exec";

    changeCameraSettings();
    changeDatabaseSettings();
    changePicDirSettings();

    settingsThread = new SettingsThread(this);
    settingsThread->setStatus(SettingsThread::STATUS_WRITE);
    settingsThread->start();

}

/*************************************更新数据 END**************************************/

void SettingsView::on_btnCapturePath_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("抓拍图路径"), "./");
    qDebug() << "file_path: " << file_path;
    ui->edDirCapture->setText(file_path + "/");
}

void SettingsView::on_btnAvatarPath_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("人脸图路径"), "./");
    qDebug() << "file_path: " << file_path;
    ui->edDirAvatar->setText(file_path + "/");
}

void SettingsView::on_btnFacePath_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("人脸子图路径"), "./");
    qDebug() << "file_path: " << file_path;
    ui->edDirFace->setText(file_path + "/");
}
