#ifndef SETTINGSTHREAD_H
#define SETTINGSTHREAD_H

#include <QtCore>
#include <QFile>
#include <QSettings>
#include <QtDebug>
#include <QThread>
#include <exception>
#include "config.h"


class SettingsThread:public QThread
{
    Q_OBJECT
public:
    explicit SettingsThread(QObject *parent = 0);

    static const int STATUS_READ = 0x0;
    static const int STATUS_WRITE = 0x1;

    void run();
    void setStatus(int status);
    bool readSettings();
    bool readCameraSettings();
    bool readDatabaseSettings();
    bool readPicDirSettings();
    bool writeCameraSettings();
    bool writeDatabaseSettings();
    bool writePicDirSettings();
    bool readAllConfig();

signals:
    void writedSettings();
    void readedSettings();
    void readedCameraSettings();
    void readedDatabaseSettings();
    bool readedPicDirSettings();

private:
    int status;
};

#endif // SETTINGSTHREAD_H
