#include "database.h"

QSqlDatabase Database::db;

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
        if(ok)
        {
            qDebug() << "Database: connect to succeed";
            return false;
        }
        else
        {
            qDebug() << "Database: connect to failed";
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
bool Database::addRecord(char* nameValue, char* sex, QString idNo, QString idCapture, QString idAvatar, bool isStranger)
{
    try{
        if(&db!=NULL) {
            //执行sql语句
            QSqlQuery query;
            QString mNameValue  = QString::fromLocal8Bit(nameValue);
            QString mSex = QString::fromLocal8Bit(sex);

            query.prepare("INSERT INTO record(nameValue, sex, idNo, idCapture, idAvatar, isStranger) "
                          "VALUES(:nameValue, :sex, :idNo, :idCapture, :idAvatar, :isStranger)");

            query.bindValue(":nameValue", mNameValue);
            query.bindValue(":sex", mSex);
            query.bindValue(":idNo", idNo);
            query.bindValue(":idCapture", idCapture);
            query.bindValue(":idAvatar", idAvatar);
            query.bindValue(":isStranger", isStranger);

            query.exec();
            return true;
        }
        return false;
    }catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}

//增加记录
bool Database::addRecord(QString nameValue, QString sex, QString idNo, QString idCapture, QString idAvatar, bool isStranger)
{
    try{
        if(&db!=NULL) {
            //执行sql语句
            QSqlQuery query;


            query.prepare("INSERT INTO record(nameValue, sex, idNo, idCapture, idAvatar, isStranger) "
                          "VALUES(:nameValue, :sex, :idNo, :idCapture, :idAvatar, :isStranger)");

            query.bindValue(":nameValue", nameValue);
            query.bindValue(":sex", sex);
            query.bindValue(":idNo", idNo);
            query.bindValue(":idCapture", idCapture);
            query.bindValue(":idAvatar", idAvatar);
            query.bindValue(":isStranger", isStranger);

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
QList<RECORD> Database::selectRecord() {
    QList<RECORD> records;
    QSqlQuery query;
    query.exec("select * from record order by timesamp desc");
    while(query.next()) {
        RECORD record;
        record.timesamp = query.value(1).toDateTime();
        record.nameValue = query.value(2).toString();
        record.sex = query.value(3).toString();
        record.idNo = query.value(4).toString();
        record.idAvatar = query.value(5).toString();
        record.idCapture = query.value(6).toString();
        record.isStranger = query.value(7).toBool();

        records.append(record);
    }
    return records;
}

void Database::setQSqlDatabase(QSqlDatabase db) {
    this->db = db;
}

QList<RECORD> Database::selectByDateTimeRange(QDateTime startDateTime, QDateTime endDateTime) {
    QList<RECORD> records;
    QSqlQuery query;

    query.prepare("select * from record where timesamp>:startDateTime and timesamp<:endDateTime");
    query.bindValue(":startDateTime", startDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":endDateTime", endDateTime.toString("yyyy-MM-dd hh:mm:ss"));

    while(query.next()) {
        RECORD record;
        record.timesamp = query.value(1).toDateTime();
        record.nameValue = query.value(2).toString();
        record.sex = query.value(3).toString();
        record.idNo = query.value(4).toString();
        record.idAvatar = query.value(5).toString();
        record.idCapture = query.value(6).toString();
        record.isStranger = query.value(7).toBool();


        records.append(record);
    }
    return records;
}
