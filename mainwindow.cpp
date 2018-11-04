#include "mainwindow.h"
#include "ui_mainwindow.h"

PreviewView *previewView = nullptr;
HistoryView *historyView = nullptr;
SettingsView *settingsView = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->tabView->setTabsClosable(true);
    ui->tabView->removeTab(0);
    ui->tabView->removeTab(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*********************************预览界面END************************************/

//创建数据库连接
bool MainWindow::createConnect(QSqlDatabase db, QString databaseType, QString hostName, int port, QString databaseName, QString userName, QString password)
{
    try {
        db = QSqlDatabase::addDatabase(databaseType);
        db.setHostName(hostName);
        db.setPort(port);
        db.setDatabaseName(databaseName);
        db.setUserName(userName);
        db.setPassword(password);

        bool ok = db.open();//建立数据库连接
        if(!ok)
        {
            qDebug() << QString::fromLocal8Bit("连接数据库失败");
            return false;
        }
        else
        {
            qDebug() << QString::fromLocal8Bit("连接数据库成功");
            return true;
        }
    } catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}

//关闭数据库连接
bool MainWindow::closeConnect(QSqlDatabase db){
    try {
        if(&db!=NULL) {
            db.close();
            return true;
        }
        return false;
    } catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}

//增加记录
bool MainWindow::addRecord(QSqlDatabase db, char* name)
{
    try{
        if(&db!=NULL) {
            //执行sql语句
            QSqlQuery query;
            QString qName  = QString::fromLocal8Bit(name);
            qDebug()<<qName;
            query.prepare("INSERT INTO record(name) "
                          "VALUES(:name)");
            query.bindValue(":name",qName);
            query.exec();
            return true;
        }
        return false;
    }catch(std::exception &e) {
        qDebug()<<"# ERR: SQLException:" <<e.what();
        //TAD：进行ui提示
    }
}
/********************************数据库操作END***********************************/

/***********************************视图****************************************/
void MainWindow::on_showPreview_triggered()
{
    if(previewView==nullptr){
        previewView = new PreviewView();
        ui->tabView->addTab(previewView, QString::fromLocal8Bit("预览"));
    }
}

void MainWindow::on_showHistory_triggered()
{
    if(historyView==nullptr) {
        historyView = new HistoryView();
        ui->tabView->addTab(historyView, QString::fromLocal8Bit("历史记录"));
    }
}

void MainWindow::on_showSettings_triggered()
{
    qDebug() << "View: Open SettingsView";
    if(settingsView==nullptr) {
        settingsView = new SettingsView();
        ui->tabView->addTab(settingsView, QString::fromLocal8Bit("设置"));
    }
    settingsView->runReadSettingsThread();

}
/**********************************视图END***************************************/
