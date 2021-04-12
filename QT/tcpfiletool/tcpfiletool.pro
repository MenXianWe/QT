#-------------------------------------------------
#
# Project created by QtCreator 2014-08-15T14:34:50
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET              = tcpfiletool
TEMPLATE            = app
MOC_DIR             = temp/moc
RCC_DIR             = temp/rcc
UI_DIR              = temp/ui
OBJECTS_DIR         = temp/obj
DESTDIR             = bin
PRECOMPILED_HEADER  = head.h

CONFIG              += warn_off
SOURCES             += main.cpp
SOURCES             += frmmain.cpp
HEADERS             += frmmain.h
FORMS               += frmmain.ui

INCLUDEPATH         += $$PWD
INCLUDEPATH         += $$PWD/api

include             ($$PWD/api/api.pri)
