#!/bin/sh
if [ ! -e ./$1 ]; then
    touch ./$@
    echo "Created file."; else
    echo "File is already created."
fi