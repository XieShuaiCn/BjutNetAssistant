#-------------------------------------------------
#
# Project created by QtCreator 2019-07-09T20:32:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BjutNetBoard
TEMPLATE = app

# BUild binary file for developer
DEFINES += BUILD_DEVELOP

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32{
RC_FILE += version.rc
}
unix{
macx{
ICON = logo.icns
}
!macx{
RC_ICONS = logo.ico
}
}

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = ../BjutNetService/common.h


SOURCES += \
    ../BjutNetService/MessageValue.cpp \
    ../BjutNetService/MessageCoder.cpp \
    ../BjutNetService/ParamParse.cpp \
    ../BjutNetService/ServiceInterface.cpp \
    ../BjutNetService/BjutNet.cpp \
    ../BjutNetService/common.cpp \
    ../BjutNetService/DebugInfo.cpp \
    ../BjutNetService/HNetworkCookieJar.cpp \
    ../BjutNetService/HttpClient.cpp \
    ../BjutNetService/WebJfself.cpp \
    ../BjutNetService/WebLgn.cpp \
    ../BjutNetService/Setting.cpp \
    main.cpp \
    ServiceBridge.cpp \
    Updater.cpp \
    HPanel.cpp \
    HLabel.cpp \
    WndMain.cpp \
    WndMain_ui.cpp\
    WndTrayIcon.cpp \
    WndSetting.cpp \
    WndSetting_ui.cpp \
    Utility.cpp \
    WndHelp.cpp \
    UISetting.cpp \
    HWidget.cpp
unix{
macx{
SOURCES += WndMain_ui_mac.cpp
}
!macx{
SOURCES += WndMain_ui_lnx.cpp
}
}
win32{
SOURCES += WndMain_ui_win.cpp
}

HEADERS += \
    ../BjutNetService/MessageValue.h \
    ../BjutNetService/MessageCoder.h \
    ../BjutNetService/ParamParse.h \
    ../BjutNetService/Version.h \
    ../BjutNetService/ServiceInterface.h \
    ../BjutNetService/BjutNet.h \
    ../BjutNetService/common.h \
    ../BjutNetService/DebugInfo.h \
    ../BjutNetService/HNetworkCookieJar.h \
    ../BjutNetService/HttpClient.h \
    ../BjutNetService/WebJfself.h \
    ../BjutNetService/WebLgn.h \
    ../BjutNetService/Setting.h \
    ServiceBridge.h \
    Updater.h \
    HLabel.h \
    HPanel.h \
    WndMain.h \
    WndTrayIcon.h \
    WndSetting.h \
    Utility.h \
    WndHelp.h \
    UISetting.h \
    HWidget.h

DISTFILES += \
    C:/Users/XIESHUAI/.bjutnet/account.json \
    /home/xieshuai/.bjutnet/account.json \
    ReadMe.txt \
    version.rc \
    config.db

RESOURCES += \
    all.qrc

include(qtsingleapplication/src/qtsingleapplication.pri)
