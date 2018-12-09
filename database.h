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

class Database
{
public:
    Database();
    QList<RECORD> setRecord(QSqlQuery query);
    QList<RECORD> selectRecord();
    QList<RECORD> selectByCondition(QDateTime startDateTime, QDateTime endDateTime, int strangerIndex, int sexIndex,
                                    int startId, int pageSize, int &totalRecordNum);
    void getTotalRecordNum(QDateTime startDateTime, QDateTime endDateTime, int strangerIndex, int sexIndex,
                                     int startId, int pageSize, int &totalRecordNum);
    bool openConnect();
    bool closeConnect();

    bool addRecord(char* nameValue, char* sex, QString idNo, QString idCapture, QString idAvatar, bool isStranger, float similar);
    bool addRecord(QString nameValue, QString sex, QString idNo, QString idCapture, QString idAvatar, bool isStranger, float similar);

    void setQSqlDatabase(QSqlDatabase db);

    static void initConfig();
private:
    static QSqlDatabase db;
    static Config config;
    static DATABASECONFIG_INFO dataBaseInfo;

};

#endif // DATABASE_H
