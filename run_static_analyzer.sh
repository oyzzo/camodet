#!/bin/sh

#Clear the analyze dir if it already exists
if [ -d analyze ]; then
	rm -rf analyze
fi

export CCC_CC=clang
export CCC_CXX=clang++

mkdir analyze && cd analyze

cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang ..
make camodet_analyze

#remember to also run cppcheck
#cppcheck -I . . --force  > /dev/null
