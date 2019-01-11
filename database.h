#ifndef DATABASE_H
#define DATABASE_H

#include <QtCore>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QList>
#include "config.h"

typedef struct tagRECORD{
    QDateTime timesamp;
    QString applicant;
    QString idAvatar;
    QString idCapture;
    bool isStranger;
} RECORD;

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

class Database
{
public:
    explicit Database();
    QList<RECORD> setRecord(QSqlQuery query);
    QList<RECORD> selectRecord();
    //根据条件查找历史纪录
    QList<RECORD> selectByCondition(QDateTime startDateTime, QDateTime endDateTime, int strangerIndex,
                                    int startId, int pageSize, int &totalRecordNum);
    void getTotalRecordNum(QDateTime startDateTime, QDateTime endDateTime, int strangerIndex,
                                     int startId, int pageSize, int &totalRecordNum);
    //根据时间与门牌号来查找记录
    QList<RECORD> selectByTimeDoorplate(QDateTime startDateTime, QDateTime endDateTime, QString doorPlate);

    void setDbConfig();
    bool openConnect();
    bool closeConnect();

    bool addRecord(QString applicant, QString idCapture, QString idAvatar, bool isStranger, float similar);

    void setQSqlDatabase(QSqlDatabase db);

    QList<House> setHouse(QSqlQuery query);

    QList<House> selectHouse(QDateTime start, QDateTime end);
    QList<House> selectHouseAsTimer();



    House selectSingleHouse();

    ApplicantInfo selectApplicantInfoBySfzNo(QString sfzNo);
    ApplicantInfo setSingleApplicantInfo(QSqlQuery query);

private:
    static QSqlDatabase db;

    //static DATABASECONFIG_INFO dataBaseInfo;

};

#endif // DATABASE_H
