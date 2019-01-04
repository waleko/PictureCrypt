#!/bin/bash

qmake "QMAKE_CXXFLAGS += -std=c++0x" "QT += gui" "QT += core" "QT += widgets" "QT += testlib" "DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900"
make all

cd src
sh ../scripts/clean.sh

cd ../tests
sh ../scripts/clean.sh

#cd ../tests
#qmake "QMAKE_CXXFLAGS += -std=c++0x" "QT += gui" "QT += core" "QT += widgets" "QT += testlib" "DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900"
#make
#sh ../scripts/clean.sh