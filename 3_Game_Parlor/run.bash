#!/bin/bash
[ -f program ] && rm program 
gcc main.c gameParlor.c -o program -lpthread -g
#./program
