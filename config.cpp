#include "config.h"

QSettings Config::configSetting("./config/config.ini", QSettings::IniFormat);
CAMERACONFIG_INFO Config::cameraInfo;
DATABASECONFIG_INFO Config::dataBaseInfo;
DIRCONFIG_INFO Config::dirInfo;
COMPARECONFIG_INFO Config::compareInfo;

Config::Config() {

}

/*************************初始化结构体************************/
void Config::initAllConfig() {
    initCameraConfig();
    initDataBaseConfig();
    initDirConfig();
    initCompareConfig();
}

void Config::initCameraConfig() {
    cameraInfo.ip = getCameraIPConfig();
    cameraInfo.port = getCameraPortConfig();
    cameraInfo.channel = getCameraChannelConfig();
    cameraInfo.userName = getCameraUserNameConfig();
    cameraInfo.passWord = getCameraPassWordConfig();
}

void Config::initDataBaseConfig() {
    dataBaseInfo.ip = getDataBaseIPConfig();
    dataBaseInfo.port = getDataBasePortConfig();
    dataBaseInfo.model = getDataBaseModelConfig();
    dataBaseInfo.userName = getDataBaseUserNameConfig();
    dataBaseInfo.passWord = getDataBasePassWordConfig();
}

void Config::initDirConfig() {
    dirInfo.dirAvatar = getDirAvatarConfig();
    dirInfo.dirCapture = getDirCaptureConfig();
    dirInfo.dirStranger = getDirStrangerConfig();
}

void Config::initCompareConfig() {
    compareInfo.similarity = getCompareSimilarityConfig();
}
/*************************初始化结构体************************/

/*******************摄像头结构体设置与读取********************/
void Config::setCameraInfoIP(QString _ip) {
    cameraInfo.ip = _ip;
}

void Config::setCameraInfoPort(int _port) {
    cameraInfo.port = _port;
}

void Config::setCameraInfoChannel(int *_channel) {
    for(int i = 0; i < 4; i++) {
        cameraInfo.channel[i] = _channel[i];
    }
}

void Config::setCameraInfoUserName(QString _userName) {
    cameraInfo.userName = _userName;
}

void Config::setCameraInfoPassWord(QString _passWord) {
    cameraInfo.passWord = _passWord;
}

QString Config::getCameraInfoIP() {
    return cameraInfo.ip;
}

int Config::getCameraInfoPort() {
    return cameraInfo.port;
}

int* Config::getCameraInfoChannel() {
    return cameraInfo.channel;
}

QString Config::getCameraInfoUserName() {
    return cameraInfo.userName;
}

QString Config::getCameraInfoPassWord() {
    return cameraInfo.passWord;
}
/*******************摄像头结构体设置与读取********************/

/*******************摄像头配置文件设置与读取*******************/
void Config::setCameraConfig() {
    setCameraIPConfig();
    setCameraPortConfig();
    setCameraChannelConfig();
    setCameraUserNameConfig();
    setCameraPassWordConfig();
}

void Config::setCameraIPConfig() {
    configSetting.setValue("/Camera/ip", cameraInfo.ip);
}

void Config::setCameraPortConfig() {
    configSetting.setValue("/Camera/port", cameraInfo.port);
}

void Config::setCameraChannelConfig() {
    configSetting.setValue("/Camera/channel1", cameraInfo.channel[0]);
    configSetting.setValue("/Camera/channel2", cameraInfo.channel[1]);
    configSetting.setValue("/Camera/channel3", cameraInfo.channel[2]);
    configSetting.setValue("/Camera/channel4", cameraInfo.channel[3]);
}

void Config::setCameraUserNameConfig() {
   configSetting.setValue("/Camera/username", cameraInfo.userName);
}
void Config::setCameraPassWordConfig() {
    configSetting.setValue("/Camera/password", cameraInfo.passWord);
}

CAMERACONFIG_INFO Config::getCameraInfoConfig() {
    initCameraConfig();
    return cameraInfo;
}

QString Config::getCameraIPConfig() {
    return configSetting.value("/Camera/ip").toString();
}

int Config::getCameraPortConfig() {
    return configSetting.value("/Camera/port").toInt();
}

int* Config::getCameraChannelConfig() {
    int *channel = new int[4];
    channel[0] = configSetting.value("/Camera/channel1").toInt();
    channel[1] = configSetting.value("/Camera/channel2").toInt();
    channel[2] = configSetting.value("/Camera/channel3").toInt();
    channel[3] = configSetting.value("/Camera/channel4").toInt();
    return channel;
}

QString Config::getCameraUserNameConfig() {
    return configSetting.value("/Camera/username").toString();
}

QString Config::getCameraPassWordConfig() {
    return configSetting.value("/Camera/password").toString();
}
/*******************摄像头配置文件设置与读取*******************/

/******************数据库结构体设置与读取******************/
void Config::setDataBaseInfoIP(QString _ip) {
    dataBaseInfo.ip = _ip;
}

void Config::setDataBaseInfoPort(int _port) {
    dataBaseInfo.port = _port;
}

void Config::setDataBaseInfoModel(QString _model) {
    dataBaseInfo.model = _model;
}

void Config::setDataBaseInfoUserName(QString _userName) {
    dataBaseInfo.userName = _userName;
}

void Config::setDataBaseInfoPassWord(QString _passWord) {
    dataBaseInfo.passWord = _passWord;
}

QString Config::getDataBaseInfoIP() {
    return dataBaseInfo.ip;
}

int Config::getDataBaseInfoPort() {
    return dataBaseInfo.port;
}

QString Config::getDataBaseInfoModel() {
    return dataBaseInfo.model;
}

QString Config::getDataBaseInfoUserName() {
    return dataBaseInfo.userName;
}

QString Config::getDataBaseInfoPassWord() {
    return dataBaseInfo.passWord;
}
/******************数据库结构体设置与读取******************/

/*******************数据库配置文件设置与读取*******************/
void Config::setDataBaseConfig() {
    setDataBaseIPConfig();
    setDataBasePortConfig();
    setDataBaseModelConfig();
    setDataBaseUserNameConfig();
    setDataBasePassWordConfig();
}

void Config::setDataBaseIPConfig() {
    configSetting.setValue("/Database/ip", dataBaseInfo.ip);
}

void Config::setDataBasePortConfig() {
    configSetting.setValue("/Database/port", dataBaseInfo.port);
}


void Config::setDataBaseModelConfig() {
    configSetting.setValue("/Database/model", dataBaseInfo.model);
}

void Config::setDataBaseUserNameConfig() {
    configSetting.setValue("/Database/username", dataBaseInfo.userName);
}

void Config::setDataBasePassWordConfig() {
    configSetting.setValue("/Database/password", dataBaseInfo.passWord);
}

DATABASECONFIG_INFO Config::getDataBaseInfoConfig() {
    initDataBaseConfig();
    return dataBaseInfo;
}

QString Config::getDataBaseIPConfig() {
    return configSetting.value("/Database/ip").toString();
}

int Config::getDataBasePortConfig() {
    return configSetting.value("/Database/port").toInt();
}

QString Config::getDataBaseModelConfig() {
    return configSetting.value("/Database/model").toString();
}

QString Config::getDataBaseUserNameConfig() {
    return configSetting.value("/Database/username").toString();
}

QString Config::getDataBasePassWordConfig() {
    return configSetting.value("/Database/password").toString();
}
/*******************数据库配置文件设置与读取*******************/

/******************文件路径结构体设置与读取******************/
void Config::setDirInfoAvatar(QString _avatar) {
    dirInfo.dirAvatar = _avatar;
}

void Config::setDirInfoCapture(QString _capture) {
    dirInfo.dirCapture = _capture;
}

void Config::setDirInfoStranger(QString _stranger) {
    dirInfo.dirStranger = _stranger;
}

QString Config::getDirInfoAvatar() {
    return dirInfo.dirAvatar;
}

QString Config::getDirInfoCapture() {
    return dirInfo.dirCapture;
}

QString Config::getDirInfoStranger() {
    return dirInfo.dirStranger;
}
/******************文件路径结构体设置与读取******************/

/******************文件路径配置文件设置与读取******************/
void Config::setDirConfigConfig() {
    setDirAvatarConfig();
    setDirCaptureConfig();
    setDirStrangerConfig();
}

void Config::setDirAvatarConfig() {
    configSetting.setValue("/Dir/dirAvatar", dirInfo.dirAvatar);
}

void Config::setDirCaptureConfig() {
    configSetting.setValue("/Dir/dirCapture", dirInfo.dirCapture);
}

void Config::setDirStrangerConfig( ) {
    configSetting.setValue("/Dir/dirStranger", dirInfo.dirStranger);
}

QString Config::getDirAvatarConfig() {
    return configSetting.value("/Dir/dirAvatar").toString();
}

DIRCONFIG_INFO Config::getDirInfoConfig() {
    initDirConfig();
    return dirInfo;
}

QString Config::getDirCaptureConfig() {
    return configSetting.value("/Dir/dirCapture").toString();
}

QString Config::getDirStrangerConfig() {
    return configSetting.value("/Dir/dirStranger").toString();
}
/******************文件路径配置文件设置与读取******************/

/*******************相似度结构体设置与读取*******************/
void Config::setCompareInfoSimilarity(float _similarity) {
    compareInfo.similarity = _similarity;
}

float Config::getCompareInfoSimilarity() {
    return compareInfo.similarity;
}
/*******************相似度结构体设置与读取*******************/

/*******************相似度配置文件设置与读取*******************/
void Config::setCompareIConfig() {
    setCompareSimilarityConfig();
}

void Config::setCompareSimilarityConfig() {
    configSetting.setValue("/Compare/similarity", compareInfo.similarity);
}

COMPARECONFIG_INFO Config::getCompareInfoConfig() {
    initCameraConfig();
    return compareInfo;
}

float Config::getCompareSimilarityConfig() {
    return configSetting.value("/Compare/similarity").toFloat();
}
/*******************相似度配置文件设置与读取*******************/





