#include "config.h"

QSettings Config::configSetting("./config/config.ini", QSettings::IniFormat);

Config::Config() {

}

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

void Config::setCompareSimilarity(float _similarity) {
    configSetting.setValue("/Compare/similarity", _similarity);
}

float Config::getCompareSimilarity() {
    return configSetting.value("/Compare/similarity").toFloat();
}
