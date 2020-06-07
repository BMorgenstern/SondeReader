#-------------------------------------------------
#
# Project created by QtCreator 2020-01-30T13:14:35
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DESTDIR = $$_PRO_FILE_PWD_

TARGET = SondeReader
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/serialio.cpp \
    src/sondedata.cpp \
    src/sondemode.cpp \
    src/sensordetail.cpp \
    src/sensorspecific.cpp \
    src/serialreader.cpp

HEADERS  += src/mainwindow.h \
    src/serialio.h \
    src/sondedata.h \
    src/sondemode.h \
    src/sensordetail.h \
    src/sensorspecific.h \
    src/serialreader.h

FORMS    += src/mainwindow.ui


