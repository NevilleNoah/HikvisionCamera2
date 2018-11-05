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

    bool Database::createConnect(QSqlDatabase db, QString databaseType, QString hostName, int port, QString databaseName, QString userName, QString password);
    bool Database::closeConnect(QSqlDatabase db);
    bool Database::addRecord(QSqlDatabase db, char* name);

};

#endif // DATABASE_H
