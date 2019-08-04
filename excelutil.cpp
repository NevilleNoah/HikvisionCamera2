#include "excelutil.h"
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QAxObject>
#include <QDir>
#include <QThread>
#include "qt_windows.h"
ExcelUtil::ExcelUtil(QObject *parent):QObject(parent)
{

}

void ExcelUtil::setPath(QString det) {
    detFileName = det;
}

void ExcelUtil::startTrans() {
    qDebug() << "child: " << QThread::currentThreadId();
    if(detFileName.isEmpty()) {
       // emit errorSignal(QStringLiteral("选择文件不能为空！"));
        return;
    }
    getData();//获取数据
    writeSheet();//写入excel
}

void ExcelUtil::getData() {
    m_oDate.clear();
    //QString str(QStringLiteral("正在解析数据..."));
    //emit progressData(0, str);
    for(int i = 0; i < 100; ++i) {
        QList<QVariant> coluDate;
        //emit progressData(i*100/100-1, str);
        for(int j = 0; j < 20; ++j) {
            coluDate << QString::number(i) + "_" + QString::number(j);
        }
        m_oDate <<coluDate;
    }
}

void ExcelUtil::writeSheet() {
    //多线程必须初始化
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    //QString str(QStringLiteral("正在写入Excel数据..."));
    //emit progressData(0, str);
    QAxObject excel("Excel.Application");
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Add");
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
    if(m_oDate.size() <= 0) return;
    if(work_sheet == nullptr || work_sheet->isNull()) return;
    int row = m_oDate.size();
    int col = m_oDate[0].size();
    for(int i = 1; i <= row; ++i) {
        int n = i * 100.0 / row;
        //emit progressData(n, str);
        for(int j = 1; j <= col; ++j) {
            QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", i, j);
            cell->setProperty("Value", m_oDate[i-1][j-1]);
        }
    }
    work_book->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(detFileName));  //另存为另一个文件
    excel.dynamicCall("Quit(void)", false);
    //emit finish();
}
