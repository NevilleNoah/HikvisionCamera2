#ifndef PREVIEWVIEW_H
#define PREVIEWVIEW_H

#include <QWidget>
#include <QtCore>
#include <QMainWindow>
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include "Windows.h"
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
#include "database.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QtNetwork>


typedef struct tagALARM_INFO{
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

    static QList<char*> avatarList;
    static QList<char*> captureList;
    static QString currentAlarmInfo;

    static int currentRow;
    static double SIMILARITY;
    static QString dirAvatar;
    static QString dirCapture;
    static QString dirPicAvatar;
    static QString dirPicCapture;
    static QString urlCapture;
    static QString urlAvatar;

    void initAlarmTable();

    static BOOL CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser);
    static void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
    static void convertUnCharToStr(BYTE *UnChar,char *hexStr, char *str, int len);
    static void setAlarmInfo(NET_VCA_FACESNAP_MATCH_ALARM struFaceMatchAlarm);
    static void setAlarmInfo();
    static void setAlarmText();
    static void saveToDatabase();

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

private:
    Ui::PreviewView *ui;
};



#endif // PREVIEWVIEW_H
