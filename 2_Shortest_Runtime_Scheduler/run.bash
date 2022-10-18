#!/bin/bash
# This file will build the project
if [[ -f program ]]; then
    ./program input.txt
else
    gcc prime.c primeFunctions.c -o prime
    gcc main.c schedulerFunctions.c -o program
    ./program input.txt
fi

