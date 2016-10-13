#!/bin/bash
# Just compare two files are the same or not.

if [ "$#" -ne 2 ]; then
    echo "Usage: bash compare.sh <file1> <file2>."
    exit 1;
fi;

diff $1 $2
r=$?
if [ $r != 0 ]; then
    echo "Incorrect!"
    exit 1;
else
    echo "Passed!"
fi;
