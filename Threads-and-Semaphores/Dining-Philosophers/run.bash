#!/bin/bash
if [ ! -f Dining-Philosophers ]; then
    gcc main.c philosopherFunc.c -o Dining-Philosophers
fi

./Dining-Philosophers

