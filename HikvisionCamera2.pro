#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T11:22:17
#
#-------------------------------------------------

QT += core gui
QT += sql
QT += network
QT += uitools
QT += xml
QT += axcontainer
# QT += xlsx
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
    excelutil.cpp \
    changeview.cpp \
        main.cpp \
        mainwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    previewview.cpp \
    historyview.cpp \
    searchview.cpp \
    settingsview.cpp \
    settingsthread.cpp \
    database.cpp \
    config.cpp \
    houseview.cpp \
    treechoose.cpp \
    xmlset.cpp  \
    excelutil.cpp

HEADERS += \
    changeview.h \
    excelutil.h \
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
    searchview.h \
    settingsview.h \
    settingsthread.h \
    database.h \
    config.h \
    houseview.h \
    headinfo.h \
    treechoose.h \
    xmlset.h  \
    excelutil.h


FORMS += \
    changeview.ui \
        mainwindow.ui \
    previewview.ui \
    historyview.ui \
    searchview.ui \
    settingsview.ui \
    houseview.ui \
    treechoose.ui
# Hikvision Camera SDK
LIBS += $$PWD/Library/GdiPlus.lib
LIBS += $$PWD/Library/HCCore.lib
LIBS += $$PWD/Library/HCNetSDK.lib
LIBS += $$PWD/Library/PlayCtrl.lib

# Config
# CONFIG += uitools

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.ini

RESOURCES += \
    configResource.qrc
