#!/bin/bash
qmake ./src/PictureCrypt.pro
make debug

# Don't run GUI application on Travis
#./travis_qmake_gcc_cpp98_boost_test_qt

# qmake travis_qmake_gcc_cpp98_boost_test_qt.pro
# make release

# Don't run GUI application on Travis
# ./travis_qmake_gcc_cpp98_boost_test_qt

# qmake travis_qmake_gcc_cpp98_boost_test_qt_test.pro
# make debug
# ./travis_qmake_gcc_cpp98_boost_test_qt_test
#
# qmake travis_qmake_gcc_cpp98_boost_test_qt_test.pro
# make release
# ./travis_qmake_gcc_cpp98_boost_test_qt_test
