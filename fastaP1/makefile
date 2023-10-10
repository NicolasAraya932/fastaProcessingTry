#!/usr/bin/make -f

location = /content/drive/MyDrive/TareaPMD/

objects = main.c file_p.c hash.c test.c kmers.c

parser: $(objects)
	gcc -g -o parser $(objects) -I $(location) -lm
