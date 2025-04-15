#!/bin/bash
unset LD_LIBRARY_PATH
doxygen CustomDoxyfile
#export LD_LIBRARY_PATH=/tools/Xilinx/Vitis/2021.2/lib/lnx64.o/Ubuntu:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
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
