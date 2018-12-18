#include "database.h"

QSqlDatabase Database::db;

Database::Database() {

}

void Database::setQSqlDatabase(QSqlDatabase db) {
    this->db = db;
}

void Database::setDbConfig() {
    db.setHostName(Config::getDataBaseInfoIP());
    db.setPort(Config::getDataBaseInfoPort());
    db.setDatabaseName(Config::getDataBaseInfoModel());
    db.setUserName(Config::getDataBaseInfoUserName());
    db.setPassword(Config::getDataBaseInfoPassWord());
}

//创建数据库连接
bool Database::openConnect() {
    try {
        setDbConfig();

        bool ok = db.open();//建立数据库连接
        if(ok) {
            qDebug() << "Database: connect to succeed";
            return false;
        }
        else {
            qDebug() << "Database: connect to failed";
            return true;
        }
    } catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}

//关闭数据库连接
bool Database::closeConnect() {
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
bool Database::addRecord(char* nameValue, char* sex, QString idCapture,
                         QString idAvatar, bool isStranger, float similar) {
    try{
        if(&db!=NULL) {
            //执行sql语句
            QSqlQuery query;
            QString mNameValue  = QString::fromLocal8Bit(nameValue);
            QString mSex = QString::fromLocal8Bit(sex);

            query.prepare("INSERT INTO record(app_name, sex, capture_id, avatar_id, stranger, similar) "
                          "VALUES(:nameValue, :sex, :idCapture, :idAvatar, :isStranger, :similar)");

            query.bindValue(":nameValue", mNameValue);
            query.bindValue(":sex", mSex);
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
bool Database::addRecord(QString nameValue, QString sex, QString idCapture,
                         QString idAvatar, bool isStranger, float similar) {
    try{
        if(&db!=NULL) {
            //执行sql语句
            QSqlQuery query;

            query.prepare("INSERT INTO record(app_name, sex, capture_id, avatar_id, stranger, similar) "
                          "VALUES(:nameValue, :sex, :idCapture, :idAvatar, :isStranger, :similar)");

            query.bindValue(":nameValue", nameValue);
            query.bindValue(":sex", sex);
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
        record.timesamp = query.value("time_value").toDateTime();
        record.nameValue = query.value("app_name").toString();
        record.sex = query.value("sex").toString();
        record.idNo = query.value("avatar_id").toString();
        record.idAvatar = query.value("avatar_id").toString();
        record.idCapture = query.value("capture_id").toString();
        record.isStranger = query.value("stranger").toBool();

        records.append(record);
    }
    return records;
}

QList<House> Database::setHouse(QSqlQuery query) {
    QList<House> houses;
    while(query.next()) {

        House house;
        house.community = query.value("community").toString();
        house.building = query.value("building").toString();
        house.unit = query.value("unit").toString();
        house.house = query.value("house").toString();
        house.area = query.value("area").toDouble();
        houses.append(house);

    }
    qDebug()<<"HouseSize::"<<houses.size();
    return houses;
}

//获取记录
QList<RECORD> Database::selectRecord() {
    QList<RECORD> records;
    QSqlQuery query;
    query.exec("select * from record order by time_value desc");
    return setRecord(query);
}


//根据出入时间获取出入情况
QList<House> Database::selectHouse(QDateTime start, QDateTime end) {
    QSqlQuery query;
    QString sqlSentence = "SELECT h.* FROM `houseapplicant` ha LEFT JOIN applicant a ON ha.applicant_id = a.id LEFT JOIN house h ON ha.house_id = h.id WHERE a.sfzno IN (SELECT DISTINCT	avatar_id	FROM	`record`	WHERE	stranger = 0	AND time_value >= :start	AND time_value <= :end)";
    query.prepare(sqlSentence);
    query.bindValue(":start","2018-12-18 16:51:34");
    query.bindValue(":end", "2018-12-18 23:59:51");
    query.exec();
    return setHouse(query);
}

//根据计时器来获取出入情况
QList<House> Database::selectHouseAsTimer() {
    QDateTime curr = QDateTime::currentDateTime();

    QDateTime start = curr.addSecs(-5);
    QDateTime end = curr.addSecs(5);

    qDebug()<<start.toString();
    qDebug()<<end.toString();

    QSqlQuery query;
    QString sqlSentence = "SELECT h.* FROM `houseapplicant` ha LEFT JOIN applicant a ON ha.applicant_id = a.id LEFT JOIN house h ON ha.house_id = h.id WHERE a.sfzno IN (SELECT DISTINCT	avatar_id	FROM	`record`	WHERE	stranger = 0	AND time_value >= :start	AND time_value <= :end)";
    query.prepare(sqlSentence);
    query.bindValue(":start", start.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":end", end.toString("yyyy-MM-dd hh:mm:ss"));
    query.exec();
    query.exec();

    return setHouse(query);
}

//获取记录总条数
void Database::getTotalRecordNum(QDateTime startDateTime, QDateTime endDateTime, int strangerIndex, int sexIndex,
                                 int startId, int pageSize, int &totalRecordNum) {
    QSqlQuery query;
    QString sqlSentence = "select count(*) from record where time_value>:startDateTime and time_value<:endDateTime";
    if(strangerIndex == 0) {
        sqlSentence += " and stranger=1";
    } else if(strangerIndex == 1) {
        sqlSentence += " and stranger=0";
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
}

//根据条件筛查记录
QList<RECORD> Database::selectByCondition(QDateTime startDateTime, QDateTime endDateTime, int strangerIndex, int sexIndex,
                                          int startId, int pageSize, int &totalRecordNum) {
    getTotalRecordNum(startDateTime, endDateTime, strangerIndex, sexIndex, startId, pageSize, totalRecordNum);
    QSqlQuery query;
    QString sqlSentence = "select * from record where time_value>:startDateTime and time_value<:endDateTime";
    if(strangerIndex == 0) {
        sqlSentence += " and stranger=1";
    } else if(strangerIndex == 1) {
        sqlSentence += " and stranger=0";
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
