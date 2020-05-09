QT += core

TARGET = LatexSymbols
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

SOURCES += main.cpp

HEADERS += ../include/QMathBran_Macros.h \
           ../include/QMathBran.h \
           ../include/QMathBranLatex_Macros.h \
           ../include/QMathBranLatex.h \
           ../include/QMathBranUnicode.h \
           ../include/QMathBranUnicode_Macros.h \
           Script_GenerateKeywords.h \
           Script_GenerateQtHeaders.h \
           Script_GenerateSymbols.h \
           Script_GenerateUnicodeScriptMaps.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += ../ConstructCodes.csv TableKeywords.csv TableSymbols.csv \
             TableUnicodeSubscripts.csv TableUnicodeSuperscripts.csv
RESOURCES += ../ConstructCodes.csv TableKeywords.csv TableSymbols.csv \
             TableUnicodeSubscripts.csv TableUnicodeSuperscripts.csv
