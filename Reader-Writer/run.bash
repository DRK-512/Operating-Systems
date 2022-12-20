#!/bin/bash
if [ ! -f output ]; then
    gcc main.c functions.c -lpthread -o output
else 
    rm output
    gcc main.c functions.c -lpthread -o output
fi

./output

