#ifndef PREVIEWVIEW_H
#define PREVIEWVIEW_H

#include <QWidget>
#include <QtCore>
#include <QMainWindow>
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include "Windows.h"
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
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
    //报警结构体
    static NET_VCA_FACESNAP_MATCH_ALARM struFaceMatchAlarm;
    //个人信息
    static char name[32];
    static char sex[2];
    //人脸库图
    static char* avatar;
    static DWORD avatarLen;
    //抓拍图
    static char* capture;
    static DWORD captureLen;
    //相似度
    static float similarity;
    //列表
    static QList<NET_VCA_FACESNAP_MATCH_ALARM> alarmList;
    static QList<char*> avatarList;
    static QList<char*> captureList;
    static QString currentAlarmInfo;

    static int currentRow;
    static double Similarity;

    void loadPreview();


    static BOOL CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser);
    static void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
    static void convertUnCharToStr(BYTE *UnChar,char *hexStr, char *str, int len);
    static void setPersonInfo(NET_VCA_FACESNAP_MATCH_ALARM struFaceMatchAlarm, int choose, int index);

signals:
    void toShowPersonInfo();
    void toAddAlarmItem();
    void toStranger();
public slots:
    void showPersonInfo();
    void addAlarmItem();
    void stranger();
private slots:
    void on_alarmList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::PreviewView *ui;
};

#endif // PREVIEWVIEW_H
