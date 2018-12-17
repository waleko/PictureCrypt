#!/bin/bash
export PATH=$HOME/PictureCrypt/mxe/usr/bin:$PATH
echo ":: qmake"
sudo ../mxe/usr/bin/i686-w64-mingw32.static-qmake-qt5 "QMAKE_CXXFLAGS += -std=c++0x" "QT += gui" "QT += core" "QT += widgets" "DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900"
echo ":: make"
make
echo ":: clean"
sh ../scripts/clean.sh
rm -rf ../mxe