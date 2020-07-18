#!/bin/bash

cd doxy

echo ":: doxygen running"
# Doxygen running
doxygen Doxyfile > doxy.log
# Generate html readme for home page
cd ..
pandoc README.md -f markdown -t html -s -o doxy/home_page/home_page_files/readme.html --email-obfuscation=javascript
# Move generated website to /
mv doxy/home_page/* ./
# Generate pdf from tex
cd latex
make
mv refman.pdf ../PictureCrypt-docs.pdf

cd ..