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
    QString name;
    QString sex;
    QString idAvatar;
    QString idCapture;
    QDateTime timesamp;
} RECORD;

class Database
{
public:
    Database();

    QList<RECORD> selectRecord();

    bool openConnect();
    bool closeConnect();
    bool addRecord(char* name, char* sex, QString idCapture, QString idAvatar, bool isStranger);
    bool addRecord(QString name, QString sex, QString idCapture, QString idAvatar, bool isStranger);

    void setQSqlDatabase(QSqlDatabase db);
    static QSqlDatabase db;

private:


};

#endif // DATABASE_H
