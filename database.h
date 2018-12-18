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
    QString nameValue;
    QString sex;
    QString idNo;
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

class Database
{
public:
    explicit Database();
    QList<RECORD> setRecord(QSqlQuery query);
    QList<RECORD> selectRecord();
    QList<RECORD> selectByCondition(QDateTime startDateTime, QDateTime endDateTime, int strangerIndex, int sexIndex,
                                    int startId, int pageSize, int &totalRecordNum);
    void getTotalRecordNum(QDateTime startDateTime, QDateTime endDateTime, int strangerIndex, int sexIndex,
                                     int startId, int pageSize, int &totalRecordNum);
    void setDbConfig();
    bool openConnect();
    bool closeConnect();

    bool addRecord(char* nameValue, char* sex, QString idCapture, QString idAvatar, bool isStranger, float similar);
    bool addRecord(QString nameValue, QString sex, QString idCapture, QString idAvatar, bool isStranger, float similar);

    void setQSqlDatabase(QSqlDatabase db);

    QList<House> setHouse(QSqlQuery query);

    QList<House> selectHouse(QDateTime start, QDateTime end);
    QList<House> selectHouseAsTimer();

    House selectSingleHouse();

private:
    static QSqlDatabase db;

    //static DATABASECONFIG_INFO dataBaseInfo;

};

#endif // DATABASE_H
