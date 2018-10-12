#!/bin/bash
cd ./src

qmake "QMAKE_CXXFLAGS += -std=c++0x" "QT += gui" "QT += core" "QT += widgets" "DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900"
make debug

qmake "QMAKE_CXXFLAGS += -std=c++0x" "QT += gui" "QT += core" "QT += widgets" "DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900"
make release
