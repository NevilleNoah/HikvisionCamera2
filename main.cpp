﻿#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <qstring.h>
#include "previewview.h"
#include "config.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    qDebug("Application start");
    Config::initAllConfig();
    qDebug() << "main.cpp" << Config::getCameraInfoIP();
    MainWindow w;
    w.setWindowTitle(QString::fromLocal8Bit("人脸识别客户端"));
    w.show();
    return a.exec();
}


