#!/bin/bash

# Doxygen running
doxygen Doxyfile

# Move pre-made index from doxy to root
mv index.html ../../index.html

# Generate pdf from tex
cd ../../latex
make
mv refman.pdf ../refman.pdf

# Deploy on windows
cd ../src
sudo apt-get install -y awscli
aws configure set aws_access_key_id $AWS_ACCESS_KEY_ID
aws configure set aws_secret_access_key $AWS_SECRET_ACCESS_KEY
aws configure set default_region_name eu-north-1
aws s3 sync s3://mxe-qt mxe
export PATH=/home/travis/PictureCrypt/mxe/usr/bin:$PATH
cd src
../mxe/usr/bin/i686-w64-mingw32.static-qmake-qt5 "QMAKE_CXXFLAGS += -std=c++0x" "QT += gui" "QT += core" "QT += widgets" "DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900"
make
sh ../scripts/clean.sh
rm -rf ../mxe