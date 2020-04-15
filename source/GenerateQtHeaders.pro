QT       += core gui widgets

TARGET = LatexSymbols
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

SOURCES += GenerateQtHeaders.cpp

HEADERS += \
    ../include/QMathBran_Macros.h \
    ../include/QMathBran.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += ../ConstructCodes.csv

RESOURCES += ../ConstructCodes.csv
