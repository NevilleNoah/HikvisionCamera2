#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include "Windows.h"
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QTabWidget>
#include <QtUiTools/QUiLoader>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QBuffer>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <qimage.h>
#include <qimagereader.h>

#include "previewview.h"
#include "historyview.h"
#include "settingsview.h"
#include "houseview.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static PreviewView* previewView;
    static HistoryView* historyView;
    static SettingsView* settingsView;
    static HouseView* houseView;

public slots:
    //void clickBtnLogin();
    //void setPersonInfo();
    //void showPreview();

signals:
    void signalPersonInfo();
    void signalMenuLogin();
    void signalMenuDB();
private slots:

private:
    /*************************************UI*****************************************/
    Ui::MainWindow *ui;
    /***********************************UI END***************************************/

    /**********************************数据库操作*************************************/
    static bool createConnect(QSqlDatabase db, QString databaseType, QString hostName, int port, QString databaseName, QString userName, QString password);
    static bool closeConnect(QSqlDatabase db);
    static bool addRecord(QSqlDatabase db, char* name);
    /********************************数据库操作 END***********************************/

};

#endif // MAINWINDOW_H
