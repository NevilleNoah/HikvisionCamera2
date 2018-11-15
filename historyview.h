#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QWidget>
#include "database.h"
namespace  Ui{
class HistoryView;
}

class HistoryView : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryView(QWidget *parent = nullptr);
    ~HistoryView();

    void initTable();
    void initData();
    void initTimeEdit();
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

private:
    Ui::HistoryView *ui;
    int pageNum;                    //页码的数量
    int totalRecordNum;             //存储记录的条数
    static int nowPage;             //当前是第几页
    const int pageSize = 18;        //每页显示的记录条数

};

#endif // HISTORYVIEW_H
