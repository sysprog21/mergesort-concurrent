#!/bin/bash
# Repeatly run the program and record the elapsed time per iteration

if [ "$#" -ne 3 ]; then
    echo "Usage: bash repeat-test.sh <thread_count> <input_data> <iteration>"
    exit 1;
fi;

threads=$1;
input_file=$2;
iteration=$3;

output="./out/repeat-test-result.dat"

rm -rf $output;
for i in $(eval echo {1..$iteration}); do
    out=$(./sort $threads $input_file | grep '#Elapsed_time:' | cut -d' ' -f2);
    echo $out >> $output;
done;

echo "Output result to $output"
