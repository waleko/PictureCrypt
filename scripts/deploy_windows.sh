#!/bin/bash

# Deploy on windows
echo ":: Deploying on windows..."
sudo apt-get install -y awscli
#echo ":: set aws_access_key"
#aws configure set aws_access_key_id $AWS_ACCESS_KEY_ID
#echo ":: set aws_access_secret"
#aws configure set aws_secret_access_key $AWS_SECRET_ACCESS_KEY
#echo ":: set aws_region"
#aws configure set default_region_name eu-north-1
echo ":: download mxe"
echo $TEST_VALUE
aws s3 sync s3://mxe-qt/ mxe --region us-west-1
export PATH=$HOME/travis/PictureCrypt/mxe/usr/bin:$PATH
cd src
echo ":: qmake"
../mxe/usr/bin/i686-w64-mingw32.static-qmake-qt5 "QMAKE_CXXFLAGS += -std=c++0x" "QT += gui" "QT += core" "QT += widgets" "DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900"
echo ":: make"
make
echo ":: clean"
sh ../scripts/clean.sh
rm -rf ../mxe