# ------------------------------------------------------------------------------
# Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
# All rights reserved.
#
# Project: DTS
# Version: 1.0
# Brief:
#
# ------------------------------------------------------------------------------
# 2018/06/01  WeiHeng       Create this file
#
# ------------------------------------------------------------------------------

INCLUDEPATH += $$PWD

FORMS += \
    $$PWD/MainWindow.ui             \
    $$PWD/DialogStartup.ui          \
    $$PWD/DialogConfiguration.ui    \
    $$PWD/DialogConfirm.ui          \
    $$PWD/DialogSynchronize.ui      \
    $$PWD/DialogAbout.ui

HEADERS += \
    $$PWD/MainWindow.h              \
    $$PWD/DialogStartup.h           \
    $$PWD/DialogConfiguration.h     \
    $$PWD/DialogConfirm.h           \
    $$PWD/DialogSynchronize.h       \
    $$PWD/DialogAbout.h

SOURCES += \
    $$PWD/MainWindow.cpp            \
    $$PWD/DialogStartup.cpp         \
    $$PWD/DialogConfiguration.cpp   \
    $$PWD/DialogConfirm.cpp         \
    $$PWD/DialogSynchronize.cpp     \
    $$PWD/DialogAbout.cpp
