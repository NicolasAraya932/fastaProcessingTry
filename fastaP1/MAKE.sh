#!/bin/sh

n=$1
k=$2

make -f makefile

./parser ${1} ${2}
