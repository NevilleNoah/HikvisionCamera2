#include "settingsthread.h"
#include "mainwindow.h"

SettingsThread::SettingsThread(QObject *parent):
    QThread(parent)
{
    qDebug() << "Settings: SettingsThread create succeed";
}

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
        emit MainWindow::previewView->loadPreview();

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
    try
    {
        emit readedCameraSettings();
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
        emit readedDatabaseSettings();
        qDebug()<<QString::fromLocal8Bit("SettingsThread: readCameraSettings succeed");
    } catch(...) {
        qDebug()<<QString::fromLocal8Bit("SettingsThread: readCameraSettings fail");
        return false;
    }
    return true;
}

bool SettingsThread::readPicDirSettings() {
    try {
        emit readedPicDirSettings();
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
        Config::setCameraConfig();

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
        Config::setDataBaseConfig();
        qDebug() << QString::fromLocal8Bit("SettingsThread: writeDatabaseSettings succeed");
    } catch(...) {
        qDebug() << QString::fromLocal8Bit("SettingsThread: writeDatabaseSettings fail");
        return false;
    }
    return true;
}

//写路径配置
bool SettingsThread::writePicDirSettings() {
    qDebug() << "current Path: " << QDir::currentPath();
    try {
        Config::setDirConfigConfig();
        qDebug() << QString::fromLocal8Bit("SettingsThread: writePicDirSettings succeed");
    } catch(...) {
        qDebug() << QString::fromLocal8Bit("SettingsThread: writePicDirSettings fail");
        return false;
    }
    return true;
}

/***************************************写配置 END***************************************/
