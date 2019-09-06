#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QWidget>
#include "headinfo.h"
#include "database.h"
#include "changeview.h"

namespace Ui{
class SearchView;
}

class SearchView : public QWidget
{
    Q_OBJECT

    enum OperateType
    {
        Add ,
        Edit
    };

public:
    explicit SearchView(QWidget *parent = nullptr);
    ~SearchView();

    void initDatabase();
    void initUI();
    void initDataShow();
    void initTextNowPageUI();
    void initTableUI();
    void clearTable();
    void setTextNowPageVal();
    void setTextNowPageNum(int nowPageNum);
    void setTextPageNumUI(int toltalPageNum);

    QDateTime getEdStartTime();
    QDateTime getEdEndTime();

    QString getEdIdNumber();//获取用户输入的身份证编号
    QString getEdName();//获取用户输入的姓名

    int calPageNum();
    int getCmbStrangerIndex();
    int getCmbSexIndex();

    int getRecordByPageNum(int startId);

    static Database database;
    static QList<ApplicantInfo> applicantInfos;
    static QString idNumber;
    static QString Name;

private:
    Ui::SearchView *ui;

    static int pageNum;             //页码数量
    static int totalRecordNum;      //存储记录条数
    static int nowPage;             //当前页数

    const int pageSize = 18;        //每页显示记录条数
    const static int fieldNum;      //字段数量

    ChangeView* changeview;
    SearchView* searchView;
    OperateType operateType;

signals:
    void signaloperateInfo(OperateType operateType);

public slots:
    void showDataByCondition();
    QStringList getCurrentRowData();
private slots:
    void on_Edit_clicked();
    void on_btnPrePage_3_clicked();
    void on_btnNextPage_3_clicked();
    void on_btnJopToPage_clicked();
    void on_btnCondition_clicked();
    void on_tableRecord_itemClicked(QTableWidgetItem *item);
    void on_AddItem_clicked();
    void on_DelItem_clicked();
};

#endif // SEARCHVIEW_H
