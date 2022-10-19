#!/bin/bash
if [ ! -f program ]; then
    gcc main.c gameParlor.c -o program -lpthread
fi
./program
