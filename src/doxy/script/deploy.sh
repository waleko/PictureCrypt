#!/bin/bash

# Doxygen running
doxygen Doxyfile

cd ../../latex
make
mv refman.pdf ../refman.pdf
