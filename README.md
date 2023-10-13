# Easy Fasta Processing with OpenMP

This is a prototype with the potential for future optimization.

The latest version is in FastaP3.

This code was shamefully written by NicolasAraya932.

## Steps

file_p.c: This code is responsible for processing the data from a .fasta file extension. It parses and extracts relevant information from the input file.

hash.c: An easy implementation of hash.

kmers.c: In this file, we extract k-mers from the sequences contained in the .fasta file. It plays a crucial role in analyzing genetic or biological data.

main.c: The main program file where the processed .fasta data is filtered and additional operations are performed. K-mers are also saved to the hash buffer for storage and retrieval.


