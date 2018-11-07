#ifndef DATABASE_H
#define DATABASE_H

#include <QtCore>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QList>

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

    QList<RECORD> selectRecord();
    QList<RECORD> selectByDateTimeRange(QDateTime startDateTime, QDateTime endDateTime);

    bool openConnect();
    bool closeConnect();
    bool addRecord(char* nameValue, char* sex, QString idNo, QString idCapture, QString idAvatar, bool isStranger);
    bool addRecord(QString nameValue, QString sex, QString idNo, QString idCapture, QString idAvatar, bool isStranger);

    void setQSqlDatabase(QSqlDatabase db);
    static QSqlDatabase db;

private:


};

#endif // DATABASE_H
