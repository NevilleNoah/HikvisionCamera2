#include "settingsthread.h"

SettingsThread::SettingsThread(QObject *parent):
    QThread(parent)
{
    qDebug() << "Settings: SettingsThread create succeed";
}

/*****************************************成员变量*****************************************/
QString SettingsThread::CMIp;
int SettingsThread::CMPort;
QString SettingsThread::CMUsername;
QString SettingsThread::CMPassword;

QString SettingsThread::DBIp;
int SettingsThread::DBPort;
QString SettingsThread::DBModel;
QString SettingsThread::DBUsername;
QString SettingsThread::DBPassword;

QString SettingsThread::dirCapture;
QString SettingsThread::dirAvatar;
/***************************************成员变量 END***************************************/


void SettingsThread::run() {
    switch(SettingsThread::status) {
    case SettingsThread::STATUS_READ:
        qDebug() << "SettingsThread: read start";

        if(readSettings()) {
            qDebug() << QString::fromLocal8Bit("Settings: read config succeed");
        } else {
            qDebug() << QString::fromLocal8Bit("Settings: read config fail");
        }
        break;
    case SettingsThread::STATUS_WRITE:
        qDebug() << "SettingsThread: write start";

        writeCameraSettings();
        writeDatabaseSettings();
        writePicDirSettings();

        emit writedSettings();
        break;
    default:
        break;
    }
}

//设置状态
void SettingsThread::setStatus(int status) {
    this->status = status;
}

/*****************************************读配置*****************************************/

//读配置
bool SettingsThread::readSettings() {
    if(readCameraSettings()&&readDatabaseSettings()&&readPicDirSettings()) {
        emit readedSettings();
        return true;
    }
    return false;
}

//读摄像机配置
bool SettingsThread::readCameraSettings() {

    try {
        QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);

        CMIp = config->value("/Camera/ip").toString();
        CMPort = config->value("/Camera/port").toInt();
        CMUsername = config->value("/Camera/username").toString();
        CMPassword = config->value("/Camera/password").toString();

        delete config;

        emit readedCameraSettings(CMIp, CMPort, CMUsername, CMPassword);

        qDebug()<<QString::fromLocal8Bit("SettingsThread: readCameraSettings succeed");
    } catch(...) {
        qDebug()<<QString::fromLocal8Bit("SettingsThread: readCameraSettings fail");
        return false;
    }

    return true;
}

//读数据库配置
bool SettingsThread::readDatabaseSettings() {

    try {
        QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);

        DBIp = config->value("/Database/ip").toString();
        DBPort = config->value("/Database/port").toInt();
        DBModel = config->value("/Database/model").toString();
        DBUsername = config->value("/Database/username").toString();
        DBPassword = config->value("/Database/password").toString();

        delete config;

        emit readedDatabaseSettings(DBIp, DBPort, DBModel, DBUsername, DBPassword);

        qDebug()<<QString::fromLocal8Bit("SettingsThread: readCameraSettings succeed");
    } catch(...) {
        qDebug()<<QString::fromLocal8Bit("SettingsThread: readCameraSettings fail");
        return false;
    }

    return true;
}

bool SettingsThread::readPicDirSettings() {
    try {
        QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);

        dirCapture = config->value("/Dir/dirCapture").toString();
        dirAvatar = config->value("/Dir/dirAvatar").toString();

        delete config;

        emit readedPicDirSettings(dirCapture, dirAvatar);

        qDebug()<<QString::fromLocal8Bit("SettingsThread: readPicDirSettings succeed");
    } catch(...) {
        qDebug()<<QString::fromLocal8Bit("SettingsThread: readPicDirSettings fail");
        return false;
    }


    return true;
}
/***************************************读配置 END***************************************/

/*****************************************写配置*****************************************/

//写摄像头配置
bool SettingsThread::writeCameraSettings() {

    try {
        QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);

        config->setValue("/Camera/ip", CMIp);
        config->setValue("/Camera/port", CMPort);
        config->setValue("/Camera/username", CMUsername);
        config->setValue("/Camera/password", CMPassword);

        delete config;

        qDebug() << QString::fromLocal8Bit("SettingsThread: writeCameraSettings succeed");
    } catch(...) {
        qDebug() << QString::fromLocal8Bit("SettingsThread: writeCameraSettings fail");
        return false;
    }

    return true;
}

//写数据库配置
bool SettingsThread::writeDatabaseSettings() {

    try {
        QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);

        config->setValue("/Database/ip", DBIp);
        config->setValue("/Database/port", DBPort);
        config->setValue("/Database/model", DBModel);
        config->setValue("/Database/username", DBUsername);
        config->setValue("/Database/password", DBPassword);

        delete config;

        qDebug() << QString::fromLocal8Bit("SettingsThread: writeDatabaseSettings succeed");
    } catch(...) {
        qDebug() << QString::fromLocal8Bit("SettingsThread: writeDatabaseSettings fail");
        return false;
    }

    return true;
}

//写数据库配置
bool SettingsThread::writePicDirSettings() {

    try {
        QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);

        config->setValue("/Dir/dirCapture", dirCapture);
        config->setValue("/Dir/dirAvatar", dirAvatar);

        delete config;

        qDebug() << QString::fromLocal8Bit("SettingsThread: writePicDirSettings succeed");
    } catch(...) {
        qDebug() << QString::fromLocal8Bit("SettingsThread: writePicDirSettings fail");
        return false;
    }

    return true;
}

/***************************************写配置 END***************************************/
