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


LIBS += $$PWD/lib/libws2_32.a
