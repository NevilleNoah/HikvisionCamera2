#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T11:22:17
#
#-------------------------------------------------

QT += core gui
QT += sql
QT += network
QT += uitools
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HikvisionCamera2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNI NGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    previewview.cpp \
    historyview.cpp \
    settingsview.cpp \
    settingsthread.cpp \
    database.cpp

HEADERS += \
        mainwindow.h \
    DataType.h \
    DecodeCardSdk.h \
    HCNetSDK.h \
    plaympeg4.h \
    DataType.h \
    DecodeCardSdk.h \
    HCNetSDK.h \
    mainwindow.h \
    plaympeg4.h \
    previewview.h \
    historyview.h \
    settingsview.h \
    settingsthread.h \
    database.h

FORMS += \
        mainwindow.ui \
    previewview.ui \
    historyview.ui \
    settingsview.ui
# Hikvision Camera SDK
LIBS += "C:/Users/admin/Documents/GitHub/HikvisionCamera2/Library/GdiPlus.lib"
LIBS += "C:/Users/admin/Documents/GitHub/HikvisionCamera2/Library/HCCore.lib"
LIBS += "C:/Users/admin/Documents/GitHub/HikvisionCamera2/Library/HCNetSDK.lib"
LIBS += "C:/Users/admin/Documents/GitHub/HikvisionCamera2/Library/PlayCtrl.lib"

# Config
#CONFIG += uitools

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.ini

RESOURCES += \
    configResource.qrc
