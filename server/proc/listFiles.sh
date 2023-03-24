#!/bin/sh
if ls $@ ; then :
    else echo "Command failed. Try giving correct path."
fi