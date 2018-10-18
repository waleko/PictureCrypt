#!/bin/bash

# Doxygen running
doxygen Doxyfile

# Move pre-made index from doxy to root
mv index.html ../../index.html

# Generate pdf from tex
cd ../../latex
make
mv refman.pdf ../refman.pdf
