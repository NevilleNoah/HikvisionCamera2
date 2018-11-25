#include "database.h"

QSqlDatabase Database::db;

Database::Database() {

}

//创建数据库连接
bool Database::openConnect()
{
    try {
        QSettings *config = new QSettings("./config/config.ini", QSettings::IniFormat);

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
bool Database::closeConnect()
{
    try {
        if(db.isOpen()) {
            db.close();
            qDebug() << "db has closed!";
        }

    } catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        return false;
        //TAD：进行ui提示
    }
    return true;
}

//增加记录
bool Database::addRecord(char* nameValue, char* sex, QString idNo, QString idCapture, QString idAvatar, bool isStranger, float similar)
{
    try{
        if(&db!=NULL) {
            //执行sql语句
            QSqlQuery query;
            QString mNameValue  = QString::fromLocal8Bit(nameValue);
            QString mSex = QString::fromLocal8Bit(sex);

            query.prepare("INSERT INTO record(nameValue, sex, idNo, idCapture, idAvatar, isStranger, similar) "
                          "VALUES(:nameValue, :sex, :idNo, :idCapture, :idAvatar, :isStranger, :similar)");

            query.bindValue(":nameValue", mNameValue);
            query.bindValue(":sex", mSex);
            query.bindValue(":idNo", idNo);
            query.bindValue(":idCapture", idCapture);
            query.bindValue(":idAvatar", idAvatar);
            query.bindValue(":isStranger", isStranger);
            query.bindValue(":similar", similar);

            query.exec();
            return true;
        }
        return false;
    } catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}

//增加记录
bool Database::addRecord(QString nameValue, QString sex, QString idNo, QString idCapture, QString idAvatar, bool isStranger, float similar)
{
    try{
        if(&db!=NULL)
        {
            //执行sql语句
            QSqlQuery query;

            query.prepare("INSERT INTO record(nameValue, sex, idNo, idCapture, idAvatar, isStranger, similar) "
                          "VALUES(:nameValue, :sex, :idNo, :idCapture, :idAvatar, :isStranger, :similar)");

            query.bindValue(":nameValue", nameValue);
            query.bindValue(":sex", sex);
            query.bindValue(":idNo", idNo);
            query.bindValue(":idCapture", idCapture);
            query.bindValue(":idAvatar", idAvatar);
            query.bindValue(":isStranger", isStranger);
            query.bindValue(":similar", similar);

            query.exec();
            return true;
        }
        return false;
    }catch(std::exception &e)
    {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}

//设置记录内容
QList<RECORD> Database::setRecord(QSqlQuery query) {
    QList<RECORD> records;
    while(query.next())
    {
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

//获取记录
QList<RECORD> Database::selectRecord()
{
    QList<RECORD> records;
    QSqlQuery query;
    query.exec("select * from record order by timeValue desc");
    return setRecord(query);
}


void Database::setQSqlDatabase(QSqlDatabase db)
{
    this->db = db;
}

//根据条件筛查记录
QList<RECORD> Database::selectByCondition(QDateTime startDateTime, QDateTime endDateTime, int strangerIndex, int sexIndex,
                                          int startId, int pageSize, int &totalRecordNum)
{
    QList<RECORD> records;
    QSqlQuery query;
    QString sqlSentence = "select count(*) from record where timeValue>:startDateTime and timeValue<:endDateTime";
    if(strangerIndex == 0) {
        sqlSentence += " and isStranger=1";
    } else if(strangerIndex == 1) {
        sqlSentence += " and isStranger=0";
        if(sexIndex == 0) {
            sqlSentence += " and sex='" + QString::fromLocal8Bit("男") + "'";
        } else if(sexIndex == 1){
            sqlSentence += " and sex='" + QString::fromLocal8Bit("女") + "'";
        }
    }
    query.prepare(sqlSentence);
    query.bindValue(":startDateTime", startDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":endDateTime", endDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    query.exec();
    query.next();
    totalRecordNum = query.value(0).toInt();

    sqlSentence = "select * from record where timeValue>:startDateTime and timeValue<:endDateTime";
    if(strangerIndex == 0) {
        sqlSentence += " and isStranger=1";
    } else if(strangerIndex == 1) {
        sqlSentence += " and isStranger=0";
        if(sexIndex == 0) {
            sqlSentence += " and sex='" + QString::fromLocal8Bit("男") + "'";
        } else if(sexIndex == 1){
            sqlSentence += " and sex='" + QString::fromLocal8Bit("女") + "'";
        }
    }
    sqlSentence += " limit :startId, :pageSize";
    query.prepare(sqlSentence);
    query.bindValue(":startDateTime", startDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":endDateTime", endDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":startId", (startId-1)*pageSize);
    query.bindValue(":pageSize", pageSize);
    query.exec();
    return setRecord(query);
}
