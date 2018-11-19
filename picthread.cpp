#include "picthread.h"

/*PicThread::PicThread(QObject *parent):
    QThread(parent)
{

}*/

PicThread::PicThread(const QString dir, const QByteArray bytes){
    this->setDir(dir);
    this->setBytes(bytes);
}

void PicThread::setDir(QString dir) {
    this->dir = dir;
}

void PicThread::setBytes(QByteArray bytes) {
    this->bytes = bytes;
}

void PicThread::run() {

}

