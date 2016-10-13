#!/bin/bash
# This is the script for generating random numbers
# and output them into a file.

n=$1;
output=$2;

if [ "$#" -ne 2 ]; then
	echo "Usage: bash gen_random_numbers.sh <count> <output>."
	exit 1;
fi;

rm -rf $output
for i in $(eval echo {1..$n}); do
	echo $RANDOM >> $output;
done;
