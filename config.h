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
typedef struct tagDIRCONFIG_INFO {
    QString dirAvatar;
    QString dirCapture;
    QString dirStranger;
} DIRCONFIG_INFO;

//相似度配置
typedef struct tagCOMPARECONFIG_INFO {
    float similarity;
} COMPARECONFIG_INFO;

class Config
{
public:
    Config();
    /******************摄像头结构体设置与读取******************/
    static void setCameraInfoIP(QString _ip);
    static void setCameraInfoPort(int _port);
    static void setCameraInfoChannel(int *_channel);
    static void setCameraInfoUserName(QString _userName);
    static void setCameraInfoPassWord(QString _passWord);

    static QString getCameraInfoIP();
    static int getCameraInfoPort();
    static int* getCameraInfoChannel();
    static QString getCameraInfoUserName();
    static QString getCameraInfoPassWord();
    /******************摄像头结构体设置与读取******************/

    /*******************摄像头配置设置与读取*******************/
    static void setCameraConfig();
    static void setCameraIPConfig();
    static void setCameraPortConfig();
    static void setCameraChannelConfig();
    static void setCameraUserNameConfig();
    static void setCameraPassWordConfig();

    static QString getCameraIPConfig();
    static int getCameraPortConfig();
    static int* getCameraChannelConfig();
    static QString getCameraUserNameConfig();
    static QString getCameraPassWordConfig();
    /*******************摄像头配置设置与读取*******************/

    /******************数据库结构体设置与读取******************/
    static void setDataBaseInfoIP(QString _ip);
    static void setDataBaseInfoPort(int _port);
    static void setDataBaseInfoModel(QString _model);
    static void setDataBaseInfoUserName(QString _userName);
    static void setDataBaseInfoPassWord(QString _passWord);

    static QString getDataBaseInfoIP();
    static int getDataBaseInfoPort();
    static QString getDataBaseInfoModel();
    static QString getDataBaseInfoUserName();
    static QString getDataBaseInfoPassWord();
    /******************数据库结构体设置与读取******************/

    /*******************数据库配置设置与读取*******************/
    static void setDataBaseConfig();
    static void setDataBaseIPConfig();
    static void setDataBasePortConfig();
    static void setDataBaseModelConfig();
    static void setDataBaseUserNameConfig();
    static void setDataBasePassWordConfig();

    static QString getDataBaseIPConfig();
    static int getDataBasePortConfig();
    static QString getDataBaseModelConfig();
    static QString getDataBaseUserNameConfig();
    static QString getDataBasePassWordConfig();
    /*******************数据库配置设置与读取*******************/

    /******************文件路径结构体设置与读取******************/
    static void setDirInfoAvatar(QString _avatar);
    static void setDirInfoCapture(QString _capture);
    static void setDirInfoStranger(QString _stranger);
    static QString getDirInfoAvatar();
    static QString getDirInfoCapture();
    static QString getDirInfoStranger();
    /******************文件路径结构体设置与读取******************/

    /******************文件路径配置设置与读取******************/
    static void setDirConfigConfig();
    static void setDirAvatarConfig();
    static void setDirCaptureConfig();
    static void setDirStrangerConfig();
    static QString getDirAvatarConfig();
    static QString getDirCaptureConfig();
    static QString getDirStrangerConfig();
    /******************文件路径配置设置与读取******************/

    /*******************相似度结构体设置与读取*******************/
    static void setCompareInfoSimilarity(float _similarity);
    static float getCompareInfoSimilarity();
    /*******************相似度结构体设置与读取*******************/

    /*******************相似度配置设置与读取*******************/
    static void setCompareIConfig();
    static void setCompareSimilarityConfig();
    static float getCompareSimilarityConfig();
    /*******************相似度配置设置与读取*******************/

    static void initAllConfig();
    static void initCameraConfig();
    static void initDataBaseConfig();
    static void initDirConfig();
    static void initCompareConfig();

    static CAMERACONFIG_INFO getCameraInfoConfig();
    static DATABASECONFIG_INFO getDataBaseInfoConfig();
    static DIRCONFIG_INFO getDirInfoConfig();
    static COMPARECONFIG_INFO getCompareInfoConfig();

private:
    static QSettings configSetting;
    static CAMERACONFIG_INFO cameraInfo;
    static DATABASECONFIG_INFO dataBaseInfo;
    static DIRCONFIG_INFO dirInfo;
    static COMPARECONFIG_INFO compareInfo;

};

#endif // CONFIG_H
