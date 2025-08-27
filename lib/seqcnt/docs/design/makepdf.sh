#!/bin/bash
doxygen CustomDoxyfile
cd ./latex
make
cd ../
if [ -n "$1" ]; then
	cp ./latex/refman.pdf ./$1.pdf
	echo "Create $1.pdf"
else
	cp ./latex/refman.pdf ./refman.pdf
	echo "Create refman.pdf"
fi
