QT += core testlib gui # For QImage

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    tst_picturecrypttest.cpp \
    ../src/modelpc.cpp \
    ../src/aes/qaesencryption.cpp

HEADERS += \
    ../src/modelpc.h \
    ../src/aes/qaesencryption.h

INCLUDEPATH += \
    ../src \
    ../src/aes

RESOURCES += \
    resource.qrc
