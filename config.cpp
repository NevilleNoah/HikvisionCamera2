#include "config.h"
#include <QDebug>

QSettings Config::configSetting("./config/config.ini", QSettings::IniFormat);
CAMERACONFIG_INFO Config::cameraInfo;
DATABASECONFIG_INFO Config::dataBaseInfo;
DIR_INFO Config::dirInfo;
COMPARECONFIG_INFO Config::compareInfo;

Config::Config() {

}


/***********************初始化配置***********************/
void Config::initAllConfig() {
    initCameraConfig();
    initDataBaseConfig();
    initDirConfig();
    initCompareConfig();
}

void Config::initCameraConfig() {
    cameraInfo.ip = getCameraIP();
    cameraInfo.port = getCameraPort();
    cameraInfo.channel = getCameraChannel();
    cameraInfo.userName = getCameraUserName();
    cameraInfo.passWord = getCameraPassWord();
}

void Config::initDataBaseConfig() {
    dataBaseInfo.ip = getDataBaseIP();
    dataBaseInfo.port = getDataBasePort();
    dataBaseInfo.model = getDataBaseModel();
    dataBaseInfo.userName = getDataBaseUserName();
    dataBaseInfo.passWord = getDataBasePassWord();
}

void Config::initDirConfig() {
    dirInfo.dirAvatar = getDirAvatar();
    dirInfo.dirCapture = getDirCapture();
    dirInfo.dirStranger = getDirStranger();
}

void Config::initCompareConfig() {
    compareInfo.similarity = getCompareSimilarity();
}
/***********************初始化配置***********************/

/*******************摄像头配置设置与读取*******************/
void Config::setCameraIP(QString _ip) {
    configSetting.setValue("/Camera/ip", _ip);
}

void Config::setCameraPort(int _port) {
    configSetting.setValue("/Camera/port", _port);
}

void Config::setCameraChannel(int *channel) {
    configSetting.setValue("/Camera/channel1", channel[0]);
    configSetting.setValue("/Camera/channel2", channel[1]);
    configSetting.setValue("/Camera/channel3", channel[2]);
    configSetting.setValue("/Camera/channel4", channel[3]);
}

void Config::setCameraUserName(QString _userName) {
   configSetting.setValue("/Camera/username", _userName);
}
void Config::setCameraPassWord(QString _passWord) {
    configSetting.setValue("/Camera/password", _passWord);
}

QString Config::getCameraIP() {
    return configSetting.value("/Camera/ip").toString();
}

int Config::getCameraPort() {
    return configSetting.value("/Camera/port").toInt();
}

int* Config::getCameraChannel() {
    int *channel = new int[4];
    channel[0] = configSetting.value("/Camera/channel1").toInt();
    channel[1] = configSetting.value("/Camera/channel2").toInt();
    channel[2] = configSetting.value("/Camera/channel3").toInt();
    channel[3] = configSetting.value("/Camera/channel4").toInt();
    return channel;
}

QString Config::getCameraUserName() {
    return configSetting.value("/Camera/username").toString();
}

QString Config::getCameraPassWord() {
    return configSetting.value("/Camera/password").toString();
}
/*******************摄像头配置设置与读取*******************/

/*******************数据库配置设置与读取*******************/
void Config::setDataBaseIP(QString _ip) {
    configSetting.setValue("/Database/ip", _ip);
}

void Config::setDataBasePort(int _port) {
    configSetting.setValue("/Database/port", _port);
}


void Config::setDataBaseModel(QString _model) {
    configSetting.setValue("/Database/model", _model);
}

void Config::setDataBaseUserName(QString _userName) {
    configSetting.setValue("/Database/username", _userName);
}

void Config::setDataBasePassWord(QString _passWord) {
    configSetting.setValue("/Database/password", _passWord);
}

QString Config::getDataBaseIP() {
    return configSetting.value("/Database/ip").toString();
}

int Config::getDataBasePort() {
    return configSetting.value("/Database/port").toInt();
}

QString Config::getDataBaseModel() {
    return configSetting.value("/Database/model").toString();
}

QString Config::getDataBaseUserName() {
    return configSetting.value("/Database/username").toString();
}

QString Config::getDataBasePassWord() {
    return configSetting.value("/Database/password").toString();
}
/*******************数据库配置设置与读取*******************/

/******************文件路径配置设置与读取******************/
void Config::setDirAvatar(QString _dirAvatar) {
    configSetting.setValue("/Dir/dirAvatar", _dirAvatar);
}

void Config::setDirCapture(QString _dirCapture) {
    configSetting.setValue("/Dir/dirCapture", _dirCapture);
}

void Config::setDirStranger(QString _dirStranger) {
    configSetting.setValue("/Dir/dirStranger", _dirStranger);
}

QString Config::getDirAvatar() {
    return configSetting.value("/Dir/dirAvatar").toString();
}

QString Config::getDirCapture() {
    return configSetting.value("/Dir/dirCapture").toString();
}

QString Config::getDirStranger() {
    return configSetting.value("/Dir/dirStranger").toString();
}
/******************文件路径配置设置与读取******************/

/*******************相似度配置设置与读取*******************/
void Config::setCompareSimilarity(float _similarity) {
    configSetting.setValue("/Compare/similarity", _similarity);
}

float Config::getCompareSimilarity() {
    return configSetting.value("/Compare/similarity").toFloat();
}
/*******************相似度配置设置与读取*******************/

CAMERACONFIG_INFO Config::getCameraInfo() {
    return cameraInfo;
}

DATABASECONFIG_INFO Config::getDataBaseInfo() {
    return dataBaseInfo;
}

DIR_INFO Config::getDirInfo() {
    return dirInfo;
}

COMPARECONFIG_INFO Config::getCompareInfo() {
    return compareInfo;
}
