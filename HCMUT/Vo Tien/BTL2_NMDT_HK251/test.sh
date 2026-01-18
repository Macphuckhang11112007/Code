#!/bin/bash

# ==========================
# Compile main.cpp
# ==========================
g++ -o main main.cpp
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# ==========================
# Run program on input.txt
# ==========================
if [ -f output.txt ]; then
    rm output.txt
fi

while IFS= read -r line; do
    echo "$line" | ./main >> output.txt
done < input.txt

# ==========================
# Compare output vs expected_output
# ==========================
if diff -w output.txt expected_output.txt > /dev/null; then
    echo "All test cases PASSED!"
else
    echo "FAIL - output.txt differs from expected_output.txt"
    echo "--- Differences ---"
    diff -w output.txt expected_output.txt
fi
