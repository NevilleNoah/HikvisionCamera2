#ifndef HOUSEVIEW_H
#define HOUSEVIEW_H

//#include <QWidget>
#include "headinfo.h"
#include "database.h"
//#include <QtCore>
//#include <QTableWidget>
//#include <QTableWidgetItem>
//#include <QString>
//#include <QTimer>
namespace Ui {
class HouseView;
}

class HouseView : public QWidget
{
    Q_OBJECT

public:
    explicit HouseView(QWidget *parent = nullptr);
    ~HouseView();

private:
    Ui::HouseView *ui;
    static Database database;
    static QList<House> houses;

    static int houseRow;
    static int houseCol;
    static int houseRowSum;
    static int houseColSum;

    static int recordRow;
    static int recordCol;
    static int recordRowSum;
    static int recordColSum;

    static QString dirAvatar;
    static QString dirCapture;
    static QString dirFace;
    static QString dirPicAvatar;
    static QString dirPicCapture;
    static QString dirPicFace;
    static QList<RECORD> records;
public:
    static void getPicDir(RECORD record);

    void initUI();
    void initDatabase();
    void initHouseTable();
    void initRecordTable();
    void initDateTime();

    void setTimer();
    void changeHouseStatus();
    void changeRecordTableData();
    void setCapturePic(QImage imgCapture);
    void setAvatarPic(QImage imgAvatar);
    void setFacePic(QImage imgFace);
    void setPersonInfo(QString applicant, QString similar, QString idAvatar, QString familyRole);
 public slots:
    void setHouseTable();
    void setRecordTable(QTableWidgetItem *item);

private slots:
    void on_flush_clicked();
    void on_houseTable_itemDoubleClicked(QTableWidgetItem *item);

    void on_recordTable_itemDoubleClicked(QTableWidgetItem *item);
    void on_exportbtn_clicked();
};

#endif // HOUSEVIEW_H
