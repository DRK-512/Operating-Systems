#!/bin/bash
[ -f program ] && rm program 
gcc main.c gameProcessing.c gameStats.c -o program -lpthread -g
#./program
