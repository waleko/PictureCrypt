QT += core testlib gui # For QImage

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += \
    tst_picturecrypttest.cpp \
    ../app/modelpc.cpp \
    ../app/aes/qaesencryption.cpp

HEADERS += \
    ../app/modelpc.h \
    ../app/aes/qaesencryption.h

INCLUDEPATH += \
    ../app \
    ../app/aes

RESOURCES += \
    resource.qrc

# Some random stuff that helps CI
QMAKE_CXXFLAGS += -std=c++0x
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900
