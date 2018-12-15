#include "mainwindow.h"
#include "ui_mainwindow.h"

PreviewView *MainWindow::previewView = nullptr;
HistoryView *MainWindow::historyView = nullptr;
SettingsView *MainWindow::settingsView = nullptr;
HouseView *MainWindow::houseView = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tabView->removeTab(0);
    ui->tabView->removeTab(0);

    if(previewView==nullptr){
        previewView = new PreviewView();
        ui->tabView->addTab(previewView, QString::fromLocal8Bit("预览"));
    }

    if(historyView==nullptr) {
        historyView = new HistoryView();
        ui->tabView->addTab(historyView, QString::fromLocal8Bit("历史记录"));
    }

    if(settingsView==nullptr) {
        settingsView = new SettingsView();
        ui->tabView->addTab(settingsView, QString::fromLocal8Bit("设置"));
    }

    if(houseView==nullptr) {
        houseView = new HouseView();
        ui->tabView->addTab(houseView, QString::fromLocal8Bit("住房状况"));
    }

    settingsView->runReadSettingsThread();
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*********************************预览界面END************************************/

/********************************数据库操作END***********************************/


