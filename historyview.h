#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QWidget>
#include "database.h"
#include <QTableWidgetItem>
namespace  Ui{
class HistoryView;
}

class HistoryView : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryView(QWidget *parent = nullptr);
    ~HistoryView();

    void initTimeEdit();

    /************************zjb**********************/
    void initTableUI();
    void initDataBase();
    void setTextNowPageNum(int nowPageNum);
    void initTextNowPageUI();
    void setTextPageNumUI(int toltalPageNum);
    void initEdStartTimeUI();
    void initEdEndTimeUI();
    void setEdStartTime(QDateTime startDateTime);
    void setEdEndTime(QDateTime endDateTime);
    QDateTime getEdStartTime();
    QDateTime getEdEndTime();
    int calPageNum();
    int getRecordByDateTimeRange(int startId);
    void initDataShow();
    void initUI();
    void clearTable();
    void setTextNowPageVal();
    void initCheckBoxUI();
    /*********************zjb END*********************/

    static Database db;
    static QList<RECORD> records;
    static QDateTime startDateTime;
    static QDateTime endDateTime;

signals:

public slots:
    void showByDateTimeRange();
private slots:

    void on_btnPrePage_clicked();

    void on_btnNextPage_clicked();

    void on_btnSearchByTime_clicked();

    void on_btnJumpPage_clicked();

    void on_recordTable_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::HistoryView *ui;
    static int pageNum;             //页码的数量
    static int totalRecordNum;      //存储记录的条数
    static int nowPage;             //当前是第几页
    const int pageSize = 18;        //每页显示的记录条数
    const static int fieldNum;         //字段数

};

#endif // HISTORYVIEW_H
