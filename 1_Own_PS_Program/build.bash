#!/bin/bash
# This file will build the project
if [[ ! -f 5ps ]]; then
    gcc 5ps.c parse_help.c -o 5ps
    echo "Use the 5ps as such: $./5ps [options]"
    echo "Please look into README for example run"
fi

