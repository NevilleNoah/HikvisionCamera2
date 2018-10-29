#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <qstring.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    qDebug("Application start");



    MainWindow w;
    w.show();
    return a.exec();
}
