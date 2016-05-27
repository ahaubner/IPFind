QT     -= core gui
CONFIG -= qt

TARGET = IPFind
TEMPLATE = lib

DEFINES += IPFIND_LIBRARY

SOURCES += \
    src/IPFind/Server.cpp \
    src/IPFind/Client.cpp \
    src/IPFind.cpp

HEADERS += \
    include/IPFind/Server.hpp \
    include/IPFind/Client.hpp \
    include/IPFind/Utils.hpp \
    include/IPFind.hpp

CONFIG -= c++11
CONFIG += c++14

INCLUDEPATH += $$PWD/include

#TargetDir
CONFIG(debug, debug|release) {
    DESTDIR = lib/debug
} else {
    DESTDIR = lib/release
}

OBJECTS_DIR = $$DESTDIR/.obj


#Depend--------------------------------------------------
unix|win32: LIBS += -L$$PWD/libs/boost/lib -lboost_system

INCLUDEPATH += $$PWD/libs/boost/include
DEPENDPATH += $$PWD/libs/boost/include

win32 {
    LIBS += -lws2_32 -lwsock32
}
