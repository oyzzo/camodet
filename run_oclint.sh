#!/bin/sh

#Clear the analyze dir if it already exists
if [ -d analyze ]; then
	rm -rf analyze
fi

mkdir analyze && cd analyze

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
oclint-json-compilation-database -- -report-type html -o report.html && firefox report.html
