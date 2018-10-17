#!/bin/bash

rm *.slo -f
rm *.lo -f
rm *.o -f
rm *.a -f
rm *.la -f
rm *.lai -f
rm *.so -f
rm *.dll -f
rm *.dylib -f

# Qt-es
rm object_script.*.Release -f
rm object_script.*.Debug -f
rm *_plugin_import.cpp -f
rm /.qmake.cache -f
rm /.qmake.stash -f
rm *.pro.user -f
rm *.pro.user.* -f
rm *.qbs.user -f
rm *.qbs.user.* -f
rm *.moc -f
rm moc_*.cpp -f
rm moc_*.h -f
rm qrc_*.cpp -f
rm ui_*.h -f
rm *.qmlc -f
rm *.jsc -f
rm Makefile* -f
rm *build-* -f

# Qt unit tests
rm target_wrapper.* -f

# QtCreator
rm *.autosave -f

# QtCreator Qml
rm *.qmlproject.user -f
rm *.qmlproject.user.* -f

# QtCreator CMake
rm CMakeLists.txt.user* -f
#TMP files
rm *.tmp -f
