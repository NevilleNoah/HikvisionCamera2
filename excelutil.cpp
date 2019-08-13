#include "excelutil.h"
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QAxObject>
#include <QDir>
#include <QThread>
#include "qt_windows.h"

Database ExcelUtil::database;
QList<ExcelExportInfo> ExcelUtil::exportInfo;
QDateTime ExcelUtil::startTime;
QDateTime ExcelUtil::endTime;

ExcelUtil::ExcelUtil(QObject *parent):QObject(parent)
{

}

void ExcelUtil::setPath(QString det) {
    detFileName = det;
}

QDateTime ExcelUtil::getStartTime() {
    return this->startTime;
}

QDateTime ExcelUtil::getEndTime() {
    return this->endTime;
}

void ExcelUtil::setStartTime(QDateTime start) {
    this->startTime = start;
}

void ExcelUtil::setEndTime(QDateTime end) {
    this->endTime = end;
}

void ExcelUtil::startTrans() {
    qDebug() << "child: " << QThread::currentThreadId();
    if(detFileName.isEmpty()) {
        // emit errorSignal(QStringLiteral("选择文件不能为空！"));
        return;
    }
    QSqlDatabase qSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    database.setQSqlDatabase(qSqlDatabase);
    getData(getStartTime(), getEndTime());//获取数据
    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    writeSheet();//写入excel
}

void ExcelUtil::getData(QDateTime start, QDateTime end) {
    exportInfo = database.selectExportRecord(start, end); //从数据库中获取需要导出的数据
    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!exportInfo.size():" << exportInfo.size();
    m_oData.clear();
    for(int i = 0; i < exportInfo.size(); ++i) {
        QList<QVariant> aRowData;
        aRowData.append(QVariant(exportInfo[i].community));
        aRowData.append(QVariant(exportInfo[i].building));
        aRowData.append(QVariant(exportInfo[i].unit));
        aRowData.append(QVariant(exportInfo[i].house));
        aRowData.append(QVariant(exportInfo[i].applicant));
        aRowData.append(QVariant(exportInfo[i].sfzno));
        aRowData.append(QVariant(exportInfo[i].familyrole));
        aRowData.append(QVariant(exportInfo[i].time_value));
        aRowData.append(QVariant(exportInfo[i].similar));
        m_oData << aRowData;
    }
}

void ExcelUtil::writeSheet() {
    //多线程必须初始化
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    QAxObject excel("Excel.Application");
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Add");
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);

    int row = m_oData.size();
    int col = m_oData[0].size();
    for(int i = 1; i <= row; ++i) {
        for(int j = 1; j <= col; ++j) {
            QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", i, j);
            cell->setProperty("Value", m_oData[i-1][j-1]);
        }
    }
    work_book->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(detFileName));
    excel.dynamicCall("Quit(void)", false);  //退出
}
