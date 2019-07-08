TARGET = BjutNetConsole
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../BjutNetService/

# BUild binary file for developer
DEFINES += BUILD_DEVELOP

CONFIG(debug, debug|release){
DEFINES += _DEBUG
DEFINES += DEBUG
}else{
DEFINES += NDEBUG
DEFINES += NO_DEBUG
}

win32{
INCLUDEPATH += E:\Programs\Boost-1.69\include
LIBS += -LE:\Programs\Boost-1.69\lib \
        -lboost_system-mgw54-mt-s-x32-1_69
        -lboost_locale-mgw54-mt-s-x32-1_69
LIBS += -lws2_32
}
unix{
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib \
        -lboost_system
LIBS += -lpthread
}

SOURCES += \
        main.cpp \
    ServiceBridge.cpp \
    UdpSocket.cpp \
    ../BjutNetService/ParamParse.cpp \
    ../BjutNetService/MessageValue.cpp \
    InterAction.cpp \
    Utility.cpp

HEADERS += \
    ServiceBridge.h \
    UdpSocket.h \
    ../BjutNetService/ParamParse.h \
    ../BjutNetService/Version.h \
    ../BjutNetService/MessageValue.h \
    InterAction.h \
    Utility.h
