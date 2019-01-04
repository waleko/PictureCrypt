#!/bin/bash

cd src
qmake "QMAKE_CXXFLAGS += -std=c++0x" "QT += gui" "QT += core" "QT += widgets" "QT += testlib" "DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900"
make
sh ../scripts/clean.sh

cd ../tests
qmake "QMAKE_CXXFLAGS += -std=c++0x" "QT += gui" "QT += core" "QT += widgets" "QT += testlib" "DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900"
make
sh ../scripts/clean.sh
