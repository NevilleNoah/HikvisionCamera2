#ifndef DATABASE_H
#define DATABASE_H

#include <QtCore>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

class Database
{
public:
    Database();

    void addRecord();
    void selectRecord();

    bool openConnect(QString hostName, int port, QString databaseName, QString userName, QString password);
    bool closeConnect();
    bool addRecord(char* name);
    void setQSqlDatabase(QSqlDatabase db);
private:
    static QSqlDatabase db;

};

#endif // DATABASE_H
