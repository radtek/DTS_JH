INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/soapH.h \
    $$PWD/soapMESServiceSoapProxy.h \
    $$PWD/soapMESServiceSoapService.h \
    $$PWD/soapStub.h \
    $$PWD/stdsoap2.h


SOURCES += \
    $$PWD/MESServiceSoap.nsmap \
    $$PWD/MESService.cpp \
    $$PWD/soapC.cpp \
    $$PWD/soapMESServiceSoapProxy.cpp \
    $$PWD/soapMESServiceSoapService.cpp \
    $$PWD/stdsoap2.cpp


LIBS += $$[QT_HOST_LIBS]/../../../Tools/mingw530_32/i686-w64-mingw32/lib/libws2_32.a
