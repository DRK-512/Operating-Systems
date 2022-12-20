#!/bin/bash
if [ ! -f output ]; then
    gcc main.c -o output
fi

./output addresses.txt process.txt

