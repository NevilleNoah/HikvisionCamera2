﻿#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include "settingsthread.h"


namespace Ui {
class SettingsView;
}

class SettingsView : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsView(QWidget *parent = nullptr);
    ~SettingsView();
    SettingsThread *settingsThread;

    void runReadSettingsThread();
    void runWriteSettingsThread();

    void changeCameraSettings();
    void changeDatabaseSettings();

public slots:
    void loadCameraSettings(QString ip, int port, QString username, QString password);
    void loadDatabaseSettings(QString ip, int port, QString model, QString username, QString password);
    void setBtnDetermineEnable();
signals:



private slots:
    void on_btnDetermine_clicked();

private:
    Ui::SettingsView *ui;
};

#endif // SETTINGSVIEW_H
