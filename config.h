#ifndef CONFIG_H
#define CONFIG_H
#include <QString>

typedef struct tagCAMERACONFIG_INFO {
    QString ip;
    int port;
    int *channel;
    QString username;
    QString password;
} CAMERACONFIG_INFO;

class Config
{
public:
    Config();
    static void setCameraIP(QString _ip);
    static void setCameraPort(int _port);
    static void setCameraChannel(int *channel);
    static void setCameraUsername(QString _username);
    static void setCameraPassword(QString _password);

    static QString getCameraIP();
    static int getCameraPort();
    static int* getCameraChannel();
    static QString getCameraUsername();
    static QString getCameraPassword();

private:
    static CAMERACONFIG_INFO cameraConfig;
};

#endif // CONFIG_H
