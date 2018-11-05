#include "database.h"

QSqlDatabase Database::db;

Database::Database() {

}

//创建数据库连接
bool Database::openConnect(QString ip, int port, QString databaseName, QString userName, QString password)
{
    try {
        db.setHostName(ip);
        db.setPort(port);
        db.setDatabaseName(databaseName);
        db.setUserName(userName);
        db.setPassword(password);

        bool ok = db.open();//建立数据库连接
        if(!ok)
        {
            qDebug() << QString::fromLocal8Bit("连接数据库失败");
            return false;
        }
        else
        {
            qDebug() << QString::fromLocal8Bit("连接数据库成功");
            return true;
        }
    } catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}

//关闭数据库连接
bool Database::closeConnect(){
    try {
        if(&db!=NULL) {
            db.close();
            return true;
        }
        return false;
    } catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}

//增加记录
bool Database::addRecord(char* name)
{
    try{
        if(&db!=NULL) {
            //执行sql语句
            QSqlQuery query;
            QString qName  = QString::fromLocal8Bit(name);
            qDebug()<<qName;
            query.prepare("INSERT INTO record(name) "
                          "VALUES(:name)");
            query.bindValue(":name",qName);
            query.exec();
            return true;
        }
        return false;
    }catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}

void Database::setQSqlDatabase(QSqlDatabase db) {
    this->db = db;
}
