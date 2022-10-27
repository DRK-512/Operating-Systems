#!/bin/bash
if [ ! -f output ]; then
    gcc main.c barberShop.c -lpthread -o output
else 
    rm output
    gcc main.c barberShop.c -lpthread -o output
fi

./output

