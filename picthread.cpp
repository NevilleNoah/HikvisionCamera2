#include "picthread.h"



/*PicThread::PicThread(const QString dir, const QByteArray bytes){

}*/



void PicThread::run() {
    /*sleep(2000);

    QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);
    QString CMUsername = config->value("/Camera/username").toString();
    QString CMPassword = config->value("/Camera/password").toString();

    QEventLoop eventLoop;
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QUrl url(urlCapture);

    url.setUserName(CMUsername);
    url.setPassword(CMPassword);
    QNetworkReply* reply = manager->get(QNetworkRequest(url));
    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    connect(manager, SIGNAL(finished(QNetworkReply*)), previewView, SLOT(showCapturePic(QNetworkReply*)));
    eventLoop.exec();*/
}

