#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include <QMetaType>
#include <QFileDialog>
#include "config.h"
#include "settingsthread.h"
#include "ui_settingsview.h"

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
    void changePicDirSettings();

public slots:
    void loadCameraSettings();
    void loadDatabaseSettings();
    void loadPicDirSettings();
    void setBtnDetermineEnable();

private slots:
    void on_btnDetermine_clicked();
    void on_btnCapturePath_clicked();
    void on_btnAvatarPath_clicked();

private:
    Ui::SettingsView *ui;
};

#endif // SETTINGSVIEW_H
