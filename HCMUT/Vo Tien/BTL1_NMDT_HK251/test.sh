#!/bin/bash

g++ -o castle main.cpp
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

rm -f output.txt

while IFS= read -r line
do
    echo "$line" | ./castle >> output.txt
done < input.txt

total_lines=$(wc -l < output.txt)
let num_tests=total_lines/4

all_passed=1

for ((i=0; i<num_tests; i++)); do
    start_line=$(( i*4 + 1 ))
    output_block=$(sed -n "${start_line},$((start_line+3))p" output.txt)
    expected_block=$(sed -n "${start_line},$((start_line+3))p" expected_output.txt)

    if [ "$output_block" = "$expected_block" ]; then
        echo "Test $((i+1)) PASS"
    else
        echo "Test $((i+1)) FAIL"
        all_passed=0
    fi
done

if [ $all_passed -eq 1 ]; then
    echo "All test cases PASSED!"
fi
