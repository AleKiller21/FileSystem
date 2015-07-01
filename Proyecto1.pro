#-------------------------------------------------
#
# Project created by QtCreator 2015-05-14T09:56:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Proyecto1
TEMPLATE = app


SOURCES += main.cpp\
        mainmenu.cpp \
    Disk.cpp \
    Lista.cpp \
    Nodo.cpp \
    mainwindow.cpp \
    creardisco.cpp

HEADERS  += mainmenu.h \
    Disk.h \
    Lista.h \
    Nodo.h \
    mainwindow.h \
    creardisco.h

FORMS    += mainmenu.ui \
    mainwindow.ui \
    creardisco.ui

QT += multimedia
QT += core
