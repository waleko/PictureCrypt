#!/bin/bash

cd src

qmake "QMAKE_CXXFLAGS += -std=c++0x" "QT += gui" "QT += core" "QT += widgets" "QT += testlib" "QT += xml" "DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900"
make all

cd app
sh ../../ci/linux/clean.sh

cd ../tests
# sh ../../ci/linux/clean.sh

cd ..
sh ../ci/linux/clean.sh

cd ..