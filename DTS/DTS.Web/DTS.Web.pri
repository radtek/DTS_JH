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

HEADERS += \
    $$PWD/soapH.h                       \
    $$PWD/soapMESServiceSoapProxy.h     \
    $$PWD/soapMESServiceSoapService.h   \
    $$PWD/soapStub.h                    \
    $$PWD/stdsoap2.h


SOURCES += \
    $$PWD/MESServiceSoap.nsmap          \
    $$PWD/MESService.cpp                \
    $$PWD/soapC.cpp                     \
    $$PWD/soapMESServiceSoapProxy.cpp   \
    $$PWD/soapMESServiceSoapService.cpp \
    $$PWD/stdsoap2.cpp


LIBS += $$PWD/lib/libws2_32.a

