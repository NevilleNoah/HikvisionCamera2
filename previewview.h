#ifndef PREVIEWVIEW_H
#define PREVIEWVIEW_H

#include <QWidget>
#include <QtCore>
#include <QMainWindow>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTabWidget>
#include <QtUiTools/QUiLoader>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QBuffer>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <qimage.h>
#include <qimagereader.h>
#include <QList>
#include <QMap>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDir>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QtNetwork>
#include <picthread.h>
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include "Windows.h"
#include "database.h"
#include "config.h"

//时间解析宏定义
#define GET_YEAR(_time_)      (((_time_)>>26) + 2000)
#define GET_MONTH(_time_)     (((_time_)>>22) & 15)
#define GET_DAY(_time_)       (((_time_)>>17) & 31)
#define GET_HOUR(_time_)      (((_time_)>>12) & 31)
#define GET_MINUTE(_time_)    (((_time_)>>6)  & 63)
#define GET_SECOND(_time_)    (((_time_)>>0)  & 63)

typedef struct tagALARM_INFO {
    DWORD dwYear;
    DWORD dwMonth;
    DWORD dwDay;
    DWORD dwHour;
    DWORD dwMinute;
    DWORD dwSecond;

    char name[NAME_LEN];
    char sex[4];
    QString id;
    float similarity;

    QString idAvatar;
    QString idCapture;
    QString sfzNo;
    QString applicant;

    bool isStranger;
}ALARM_INFO;


namespace Ui {
    class PreviewView;
}

class PreviewView : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewView(QWidget *parent = nullptr);
    ~PreviewView();
    //本类
    static PreviewView* previewView;
    //预览属性设置
    static LONG lUserID;//用户句柄
    static LONG lRealPlayHandle;//播放句柄
    //画面设置
    static WORD m_wPicSize;//预览画面大小设置
    static WORD m_wPicQuality;//预览画面质量设置
    //报警信息
    static NET_VCA_FACESNAP_MATCH_ALARM struFaceMatchAlarm;
    static ALARM_INFO alarmInfo;//存储当前报警信息
    static QList<ALARM_INFO> alarmList;//报警信息列表
    static QString alarmText;//打印输出的文本

    static QList<int> searchList;//存储搜索信息

    //人脸库图
    static char* avatar;
    static DWORD avatarLen;
    static DWORD avatarIDLen;
    //抓拍图
    static char* capture;
    static DWORD captureLen;
    //相似度
    static double similarity;
    //数据库
    static Database database;

    //是否点击搜索
    static bool isClickSearch;
    //搜索的名字
    static QString inputName;

    static QList<char*> avatarList;
    static QList<char*> captureList;
    static QString currentAlarmInfo;

    static int currentRow;
    static QString dirAvatar;
    static QString dirCapture;
    static QString dirPicAvatar;
    static QString dirPicCapture;
    static QString urlCapture;
    static QString urlAvatar;

    void initAlarmTable();
    static void initConfig();

    static BOOL CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser);
    static void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
    static void convertUnCharToStr(BYTE *UnChar,char *hexStr, char *str, int len);
    static void setAlarmInfo(NET_VCA_FACESNAP_MATCH_ALARM struFaceMatchAlarm);
    static void setAlarmInfo();
    static void setAlarmText();
    static void saveToDatabase();

    static bool isSetAlarmText();

    static void initDatabase();
private:
    //static Config config;
    //static CAMERACONFIG_INFO cameraInfo;
    //static COMPARECONFIG_INFO compareInfo;

signals:

public slots:
    void showPersonInfo(int option);
    void showCapturePic(QNetworkReply*);
    void showAvatarPic(QNetworkReply*);
    void addAlarmItem();

    void loadPreview();

private slots:
    void on_alarmList_itemDoubleClicked(QListWidgetItem *item);

    void on_btnAlarmClear_clicked();

    void on_btnSearch_clicked();

private:
    Ui::PreviewView *ui;
};



#endif // PREVIEWVIEW_H
