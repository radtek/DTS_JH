#-------------------------------------------------
#
# Project created by QtCreator 2018-01-15T09:10:24
#
#-------------------------------------------------

QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DTS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(ExgSOAP/ExgSOAP.pri)
include(ExWidget/ExWidget.pri)
include(ExCore/ExCore.pri)
include(Main/Main.pri)

DESTDIR = ../bin

RESOURCES += \
    Resources/DTS.qrc

RC_FILE = Resources/DTS.rc
