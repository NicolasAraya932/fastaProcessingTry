# Tarea 1 Procesamiento Masivo de Datos.

## Principal Idea 

In this homework the principal Idea is get every kmer from a fasta archive. The way to do it depends a lot about the idea of use different auxiliar libraries. 
The plan is to do everything we need without needing an auxiliar library like kseq. But if the libraries are requirement to deal with any kind of situation, I'm open to using them.

### Implementation Ideas:

* Use fopen from stdio.h to process the .fasta archives and then deal with the 
problem of get the length of the string of chars.

* To deal with the length of the string, make a void function which modify a global counter variable and then call that variable inside the function were I'll save the string filtered.

#### Filtering:

* Modify the global counter variable adding 1 only when the char is in "ATCG" and deleting the first line with the description of the sequence.

* Do the same, but creating a dinamic var. with the real size of the string.

### KMER:

* Use the pseudocode found in wikipedia and make a matrix with all the kmers.

### Hash:

* Make a 4^K buffer.

* Thanks to A = 00, T = 01, C = 10, G = 11 the size of the buffer should be 4^k and is perfect for storing each kmer. In this case 2^7 + 2^6 + 2^5 + 2^4 + 2^3 + 2^2 + 2^1 + 2^0 = 255 and 4^4 = 256.

* Transform each kmer to decimal number represented by the last point A = 00, T = 01, C = 10, G = 11 and then store it in the buffer[decimal]++; like that.

