#ifndef PICTHREAD_H
#define PICTHREAD_H

#include <QObject>
#include <QThread>
#include <QtCore>
#include <QtNetwork>

class PicThread:public QThread
{
    Q_OBJECT
public:
    explicit PicThread(const QString urlCapture);

    void run();
private:


};

#endif // PICTHREAD_H
