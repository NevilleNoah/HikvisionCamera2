#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include <QSettings>

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

    /*******************相似度配置设置与读取*******************/
    static void setCompareSimilarity(float _similarity);
    static float getCompareSimilarity();
    /*******************相似度配置设置与读取*******************/

private:
    static QSettings configSetting;
};

#endif // CONFIG_H
