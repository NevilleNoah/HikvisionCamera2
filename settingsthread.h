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
    static int CMChannel;
    static QString CMUsername;
    static QString CMPassword;

    static QString DBIp;
    static int DBPort;
    static QString DBModel;
    static QString DBUsername;
    static QString DBPassword;

    static QString dirCapture;
    static QString dirAvatar;

    void run();

    void setStatus(int status);

    bool readSettings();
    bool readCameraSettings();
    bool readDatabaseSettings();
    bool readPicDirSettings();

    bool writeCameraSettings();
    bool writeDatabaseSettings();
    bool writePicDirSettings();

signals:
    void writedSettings();
    void readedSettings();

    void readedCameraSettings(QString ip, int port, int channel, QString username, QString password);
    void readedDatabaseSettings(QString ip, int port, QString model, QString username, QString password);
    bool readedPicDirSettings(QString dirCapture, QString dirAvatar);

public slots:

private:
    int status;

};

#endif // SETTINGSTHREAD_H
