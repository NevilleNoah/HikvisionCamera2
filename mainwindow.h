#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HCNetSDK.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow *pMainWindow;
    static BOOL CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser);
public slots:
    void clickBtnLogin();
    void setPersonInfo();
signals:
    void signalPersonInfo();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
