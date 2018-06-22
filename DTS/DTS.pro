
QT += core gui widgets network xml sql

TARGET = DTS
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

include(DTS.Web/DTS.Web.pri)
include(DTS.Widget/DTS.Widget.pri)
include(DTS.Core/DTS.Core.pri)
include(DTS.Main/DTS.Main.pri)
include(XY.Global/XY.Global.pri)


DESTDIR = ../bin
RC_FILE = DTS.rc
RESOURCES = DTS.qrc
