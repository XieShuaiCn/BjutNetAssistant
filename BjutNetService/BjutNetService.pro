QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# BUild binary file for developer
#DEFINES += BUILD_DEVELOP
#QMAKE_CXXFLAGS += mwindows

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    ServiceInterface.cpp \
    MessageValue.cpp \
    BjutNet.cpp \
    common.cpp \
    DebugInfo.cpp \
    HNetworkCookieJar.cpp \
    HttpClient.cpp \
    WebJfself.cpp \
    WebLgn.cpp \
    Setting.cpp \
    MessageCoder.cpp

HEADERS += \
    ServiceInterface.h \
    MessageValue.h \
    Version.h \
    BjutNet.h \
    common.h \
    DebugInfo.h \
    HNetworkCookieJar.h \
    HttpClient.h \
    WebJfself.h \
    WebLgn.h \
    Setting.h \
    MessageCoder.h

include(qtsingleapplication/src/qtsingleapplication.pri)
