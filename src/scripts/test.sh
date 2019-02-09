#!/bin/bash

cd tests
QT_QPA_PLATFORM=offscreen make check # > testing.log
cd ..
