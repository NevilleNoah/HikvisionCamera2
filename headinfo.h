#ifndef HEADINFO_H
#define HEADINFO_H

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
#include <QTimer>
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

//时间解析宏定义
#define GET_YEAR(_time_)      (((_time_)>>26) + 2000)
#define GET_MONTH(_time_)     (((_time_)>>22) & 15)
#define GET_DAY(_time_)       (((_time_)>>17) & 31)
#define GET_HOUR(_time_)      (((_time_)>>12) & 31)
#define GET_MINUTE(_time_)    (((_time_)>>6)  & 63)
#define GET_SECOND(_time_)    (((_time_)>>0)  & 63)

//报警信息结构体
typedef struct tagALARM_INFO {
    DWORD dwYear;
    DWORD dwMonth;
    DWORD dwDay;
    DWORD dwHour;
    DWORD dwMinute;
    DWORD dwSecond;

    char name[NAME_LEN];
    //char sex[4];
    QString id;
    float similarity;

    QString idAvatar;
    QString idCapture;
    QString sfzNo;
    QString applicant;

    bool isStranger;
} ALARM_INFO;

//历史记录
typedef struct tagRECORD{
    QDateTime timesamp;
    QString applicant;
    QString idAvatar;
    QString idCapture;
    int similar;
    bool isStranger;
} RECORD;

//住址信息 XX小区 X幢 X单元 XX室 XX面积
typedef struct tagHouse {
    QString community;
    QString building;
    QString unit;
    QString house;
    double area;
} House;

typedef struct tagApplicantInfo {
    QString applicant;
} ApplicantInfo;

//该结构体冗余
typedef struct tagADDRESS_INFO{
    QString community;
    QString building;
    QString unit;
    QString house;
} ADDRESS_INFO;


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
    QString dirFace;
    QString dirStranger;
} DIRCONFIG_INFO;

//相似度配置
typedef struct tagCOMPARECONFIG_INFO {
    float similarity;
} COMPARECONFIG_INFO;

#endif // HEADINFO_H
