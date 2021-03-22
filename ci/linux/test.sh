#!/bin/bash

cd src/tests
QT_QPA_PLATFORM=offscreen make check
cd ../..