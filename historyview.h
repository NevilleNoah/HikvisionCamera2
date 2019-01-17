#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include "headinfo.h"
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

    void initTimeEdit();

    void initTableUI();
    void initDatabase();
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
    int getRecordByPageNum(int startId);
    void initDataShow();
    void initUI();
    void clearTable();
    void setTextNowPageVal();
    void initComboBoxUI();
    void setComboBoxState(bool strangerState, bool sexState);
    void setComboBoxUI(int strangerIndex, int sexIndex);
    int getCmbStrangerIndex();
    int getCmbSexIndex();
    void cleanImage();

    static Database database;
    static QList<RECORD> records;
    static QDateTime startDateTime;
    static QDateTime endDateTime;

signals:

public slots:
    void showDataByCondition();
private slots:

    void on_btnPrePage_clicked();

    void on_btnNextPage_clicked();

    void on_btnSearchByCondition_clicked();

    void on_btnJumpPage_clicked();

    void on_recordTable_itemDoubleClicked(QTableWidgetItem *item);

    void on_cmbStranger_currentIndexChanged(int index);

private:
    Ui::HistoryView *ui;
    static int pageNum;             //页码的数量
    static int totalRecordNum;      //存储记录的条数
    static int nowPage;             //当前是第几页

    const int pageSize = 18;        //每页显示的记录条数
    const static int fieldNum;         //字段数

};

#endif // HISTORYVIEW_H
