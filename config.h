#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include <QSettings>

//摄像头配置
typedef struct tagCAMERACONFIG_INFO {
    QString ip;
    int port;
    int *channel;
    QString userName;
    QString passWord;
} CAMERACONFIG_INFO;

//数据库配置
typedef struct tagDATABASECONFIG_INFO {
    QString ip;
    int port;
    QString model;
    QString userName;
    QString passWord;
} DATABASECONFIG_INFO;

//路径配置
typedef struct tagDIR_INFO {
    QString dirAvatar;
    QString dirCapture;
    QString dirStranger;
} DIR_INFO;

//相似度配置
typedef struct tagCOMPARECONFIG_INFO {
    float similarity;
} COMPARECONFIG_INFO;

class Config
{
public:
    Config();
    /*******************摄像头配置设置与读取*******************/
    static void setCameraIP(QString _ip);
    static void setCameraPort(int _port);
    static void setCameraChannel(int *channel);
    static void setCameraUserName(QString _userName);
    static void setCameraPassWord(QString _passWord);

    static QString getCameraIP();
    static int getCameraPort();
    static int* getCameraChannel();
    static QString getCameraUserName();
    static QString getCameraPassWord();
    /*******************摄像头配置设置与读取*******************/

    /*******************数据库配置设置与读取*******************/
    static void setDataBaseIP(QString _ip);
    static void setDataBasePort(int _port);
    static void setDataBaseModel(QString _model);
    static void setDataBaseUserName(QString _userName);
    static void setDataBasePassWord(QString _passWord);

    static QString getDataBaseIP();
    static int getDataBasePort();
    static QString getDataBaseModel();
    static QString getDataBaseUserName();
    static QString getDataBasePassWord();
    /*******************数据库配置设置与读取*******************/

    /******************文件路径配置设置与读取******************/
    static void setDirAvatar(QString _dirAvatar);
    static void setDirCapture(QString _dirCapture);
    static void setDirStranger(QString _dirStranger);
    static QString getDirAvatar();
    static QString getDirCapture();
    static QString getDirStranger();
    /******************文件路径配置设置与读取******************/

    /*******************相似度配置设置与读取*******************/
    static void setCompareSimilarity(float _similarity);
    static float getCompareSimilarity();
    /*******************相似度配置设置与读取*******************/

    static void initAllConfig();
    static void initCameraConfig();
    static void initDataBaseConfig();
    static void initDirConfig();
    static void initCompareConfig();

    static CAMERACONFIG_INFO getCameraInfo();
    static DATABASECONFIG_INFO getDataBaseInfo();
    static DIR_INFO getDirInfo();
    static COMPARECONFIG_INFO getCompareInfo();

private:
    static QSettings configSetting;
    static CAMERACONFIG_INFO cameraInfo;
    static DATABASECONFIG_INFO dataBaseInfo;
    static DIR_INFO dirInfo;
    static COMPARECONFIG_INFO compareInfo;

};

#endif // CONFIG_H
