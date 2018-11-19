#ifndef PICTHREAD_H
#define PICTHREAD_H

#include <QObject>
#include <QThread>
#include <QtCore>

class PicThread:public QThread
{
    Q_OBJECT
public:
    //explicit PicThread(QObject *parent);
    explicit PicThread(const QString dir, const QByteArray bytes);

    void setDir(QString);
    void setBytes(QByteArray);

    void run();
private:
    QString dir;
    QByteArray bytes;

};

#endif // PICTHREAD_H
