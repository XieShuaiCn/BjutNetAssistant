TARGET = BjutNetConsole
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../BjutNetService/

win32{
INCLUDEPATH += E:\Programs\Boost-1.69.gcc\include
LIBS += -LE:\Programs\Boost-1.69.gcc\lib \
        -lboost_system-mgw54-mt-s-x64-1_69
LIBS += -lws2_32
}
unix{
INCLUDEPATH += /usr/local/include
}

SOURCES += \
        main.cpp \
    ServiceBridge.cpp \
    UdpSocket.cpp \
    ../BjutNetService/ParamParse.cpp \
    ../BjutNetService/MessageValue.cpp \
    InterAction.cpp

HEADERS += \
    ServiceBridge.h \
    UdpSocket.h \
    ../BjutNetService/ParamParse.h \
    ../BjutNetService/Version.h \
    ../BjutNetService/MessageValue.h \
    InterAction.h
