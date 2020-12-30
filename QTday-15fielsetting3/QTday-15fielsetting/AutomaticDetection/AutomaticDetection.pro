#-------------------------------------------------
#
# Project created by QtCreator 2020-07-28T13:48:19
#
#-------------------------------------------------
CONFIG += c++11
QT += serialbus serialport widgets

QT       += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = QT_DiceThreah
TARGET = AutomaticDetection
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
    setdialog.cpp \
    addequipment.cpp \
    modifyaddress.cpp \
    serialport.cpp \
    libmodbus/modbus.c \
    libmodbus/modbus-data.c \
    libmodbus/modbus-rtu.c \
    libmodbus/modbus-tcp.c

HEADERS += \
        mainwindow.h \
    setdialog.h \
    addequipment.h \
    modifyaddress.h \
    serialport.h \
    struct.h \
    libmodbus/config.h \
    libmodbus/modbus.h \
    libmodbus/modbus-private.h \
    libmodbus/modbus-rtu.h \
    libmodbus/modbus-rtu-private.h \
    libmodbus/modbus-tcp.h \
    libmodbus/modbus-tcp-private.h \
    libmodbus/modbus-version.h

FORMS += \
        mainwindow.ui \
    setdialog.ui \
    addequipment.ui \
    modifyaddress.ui \
    serialport.ui
win32:LIBS += -lws2_32

DISTFILES +=

RESOURCES += \
    res.qrc
