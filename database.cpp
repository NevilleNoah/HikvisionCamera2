#include "database.h"

QSqlDatabase Database::db= QSqlDatabase::addDatabase("QMYSQL");

Database::Database() {

}

//创建数据库连接
bool Database::openConnect()
{
    try {
        QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);

        QString ip = config->value("/Database/ip").toString();
        int port = config->value("/Database/port").toInt();
        QString model = config->value("/Database/model").toString();
        QString username = config->value("/Database/username").toString();
        QString password = config->value("/Database/password").toString();

        delete config;
        db.setHostName(ip);
        db.setPort(port);
        db.setDatabaseName(model);
        db.setUserName(username);
        db.setPassword(password);

        bool ok = db.open();//建立数据库连接
        if(!ok)
        {
            qDebug() << QString::fromLocal8Bit("Database: connect to mysql succeed");
            return false;
        }
        else
        {
            qDebug() << QString::fromLocal8Bit("Database: connect to mysql failed");
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
bool Database::addRecord(char* name, char* sex, QString idCapture, QString idAvatar)
{
    try{
        if(&db!=NULL) {
            //执行sql语句
            QSqlQuery query;
            QString mName  = QString::fromLocal8Bit(name);
            QString mSex = QString::fromLocal8Bit(sex);
            QString mIdCapture = idCapture;
            QString mIdAvatar = idAvatar;

            query.prepare("INSERT INTO record(name, sex, idCapture, idAvatar) "
                          "VALUES(:name, :sex, :idCapture, :idAvatar)");

            query.bindValue(":name", mName);
            query.bindValue(":sex", mSex);
            query.bindValue(":idCapture", mIdCapture);
            query.bindValue(":idAvatar", mIdAvatar);

            query.exec();
            return true;
        }
        return false;
    }catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}

//获取记录
QList<RECORD> selectRecord() {
    QList<RECORD> records;
    return records;
}


