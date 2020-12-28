#-------------------------------------------------
#
# Project created by QtCreator 2020-12-09T11:18:45
#
#-------------------------------------------------
QT       += serialport

QT       += serialbus
QT       += core gui
win32:LIBS += -lws2_32
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = isloatingSwitch
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    switchport.cpp \
    modbus/modbus.c \
    modbus/modbus-data.c \
    modbus/modbus-rtu.c \
    modbus/modbus-tcp.c \
    switchangels.cpp

HEADERS += \
        mainwindow.h \
    switchport.h \
    modbus/config.h \
    modbus/modbus.h \
    modbus/modbus-private.h \
    modbus/modbus-rtu.h \
    modbus/modbus-rtu-private.h \
    modbus/modbus-tcp.h \
    modbus/modbus-tcp-private.h \
    modbus/modbus-version.h \
    switchangels.h

FORMS += \
        mainwindow.ui \
    switchangels.ui \
    switchport.ui

DISTFILES += \

RESOURCES += \
    images.qrc

RC_ICONS = logo.ico
