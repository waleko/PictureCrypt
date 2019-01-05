#-------------------------------------------------
#
# Project created by QtCreator 2018-07-10T20:35:44
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PictureCrypt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        viewpc.cpp \
    controllerpc.cpp \
    modelpc.cpp \
    encryptdialog.cpp \
    aboutpc.cpp \
    aes/qaesencryption.cpp

HEADERS += \
        viewpc.h \
    controllerpc.h \
    modelpc.h \
    encryptdialog.h \
    aboutpc.h \
    aes/qaesencryption.h

FORMS += \
        viewpc.ui \
    encryptdialog.ui \
    aboutpc.ui

RESOURCES += \
    resource.qrc \
    translations.qrc

CONFIG(debug, debug|release) {
    DESTDIR         = $$OUT_PWD/build/Debug
}else {
    DESTDIR         = $$OUT_PWD/build/Release
}
#win32:QMAKE_POST_LINK += windeployqt $$DESTDIR $$escape_expand(\\n)

TRANSLATIONS +=  \
    translations/picturecrypt_ru.ts \
    translations/picturecrypt_de.ts
