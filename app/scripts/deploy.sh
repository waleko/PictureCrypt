#!/bin/bash

echo ":: doxygen running"
# Doxygen running
doxygen Doxyfile > doxy.log
# Move pre-made index from doxy to root
mv index.html ../../../index.html
# Generate pdf from tex
cd ../../../latex
make
mv refman.pdf ../refman.pdf