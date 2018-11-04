#ifndef SETTINGSTHREAD_H
#define SETTINGSTHREAD_H

#include <QtCore>
#include <QFile>
#include <QSettings>
#include <QtDebug>
#include <QThread>
#include <exception>

class SettingsThread:public QThread
{
    Q_OBJECT
public:
    explicit SettingsThread(QObject *parent = 0);

    static const int STATUS_READ = 0x0;
    static const int STATUS_WRITE = 0x1;

    static QString CMIp;
    static int CMPort;
    static QString CMUsername;
    static QString CMPassword;

    static QString DBIp;
    static int DBPort;
    static QString DBUsername;
    static QString DBPassword;

    void run();

    void setStatus(int status);

    bool readSettings();
    bool readCameraSettings();
    bool readDatabaseSettings();

    bool writeCameraSettings();
    bool writeDatabaseSettings();

signals:
    void writedSettings();
    void readedSettings();

    void readedCameraSettings(QString ip, int port, QString username, QString password);
    void readedDatabaseSettings(QString ip, int port, QString username, QString password);

public slots:

private:
    int status;

};

#endif // SETTINGSTHREAD_H
