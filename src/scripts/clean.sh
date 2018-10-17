#!/bin/bash

rm *.slo
rm *.lo
rm *.o
rm *.a
rm *.la
rm *.lai
rm *.so
rm *.dll
rm *.dylib

# Qt-es
rm object_script.*.Release
rm object_script.*.Debug
rm *_plugin_import.cpp
rm /.qmake.cache
rm /.qmake.stash
rm *.pro.user
rm *.pro.user.*
rm *.qbs.user
rm *.qbs.user.*
rm *.moc
rm moc_*.cpp
rm moc_*.h
rm qrc_*.cpp
rm ui_*.h
rm *.qmlc
rm *.jsc
rm Makefile*
rm *build-*

# Qt unit tests
rm target_wrapper.*

# QtCreator
rm *.autosave

# QtCreator Qml
rm *.qmlproject.user
rm *.qmlproject.user.*

# QtCreator CMake
rm CMakeLists.txt.user*
#TMP files
rm *.tmp
