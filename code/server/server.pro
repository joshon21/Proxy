#-------------------------------------------------
#
# Project created by QtCreator 2018-06-30T20:52:29
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =  server1
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mythread.cpp \
    myserver.cpp \
    mytcpsocket.cpp

HEADERS  +=  widget.h \
     mythread.h \
    myserver.h \
    mytcpsocket.h

FORMS   += widget.ui

#OBJECTS_DIR  = tmp
#MOC_DIR      = tmp
