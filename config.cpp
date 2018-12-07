#include "config.h"



Config::Config() {

}

void Config::setCameraIP(QString _ip) {
    cameraConfig.ip = _ip;
}

void Config::setCameraPort(int _port) {
    cameraConfig.port = _port;
}

void Config::setCameraChannel(int *channel) {
    cameraConfig.channel = channel;
}

void Config::setCameraUsername(QString _username) {
    cameraConfig.username = _username;
}
void Config::setCameraPassword(QString _password) {
    cameraConfig.password = _password;
}

QString Config::getCameraIP() {
    return cameraConfig.ip;
}

int Config::getCameraPort() {
    return cameraConfig.port;
}

int* Config::getCameraChannel() {
    return cameraConfig.channel;
}

QString Config::getCameraUsername() {
    return cameraConfig.username;
}

QString Config::getCameraPassword() {
    return cameraConfig.password;
}
