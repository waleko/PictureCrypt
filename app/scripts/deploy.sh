#!/bin/bash

echo ":: doxygen running"
# Doxygen running
doxygen Doxyfile > doxy.log
# Move pre-made index from doxy to root
mv index.html ../../../index.html
# Generate html readme for home page
cd ../../..
markdown README.md > readme.html
# Generate pdf from tex
cd latex
make
mv refman.pdf ../refman.pdf
