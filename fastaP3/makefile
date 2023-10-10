#!/usr/bin/make -f

location = /home/nicoa/Desktop/fastaProyect/fastaP3/

objects = main.c file_p.c hash.c test.c kmers.c

parser: $(objects)
	gcc -g -o parser $(objects) -I $(location) -lm -fopenmp 
