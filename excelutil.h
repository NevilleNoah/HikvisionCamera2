#ifndef EXCELUTIL_H
#define EXCELUTIL_H

#include <QObject>
#include "database.h"
#include "headinfo.h"
#include <QDateTime>

class ExcelUtil : public QObject
{
     Q_OBJECT
public:
    explicit ExcelUtil(QObject *parent = nullptr);
    void setPath(QString det);
    QDateTime getStartTime();
    QDateTime getEndTime();
    void setStartTime(QDateTime start);
    void setEndTime(QDateTime end);
signals:
    //void finish();
    //void errorSignal(QString strMsg);
    //void progressData(int nValue, QString str);

private:
    static QDateTime startTime;
    static QDateTime endTime;
    static QList<ExcelExportInfo> exportInfo;
    static Database database;
    void getData(QDateTime start, QDateTime end);
    void writeSheet();

public slots:
    void startTrans();

private:
    QList<QList<QVariant>> m_oDate;
    QString detFileName;
};

#endif // EXCELUTIL_H
