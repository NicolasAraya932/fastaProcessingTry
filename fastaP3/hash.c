#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define A 0x0
#define T 0x1
#define C 0x2
#define G 0x3

unsigned long int from_kmer_to_binary(char *c)
{
  unsigned long int result = 0;

  while(*c != '\0'){
    unsigned int x = (*c == 'A') ? A : \
        (*c == 'T') ? T : \
        (*c == 'C') ? C : \
        (*c == 'G') ? G : \
        99;
    if(x == 99){
      errno = EPROTO;
      perror("There is an invalid char");
      exit(1);
    }
    result |= x;
    result <<= 2;
    c++;
  }
  result >>= 2;

  return result;
}

unsigned long int hash_function(char *kmer)
{
    return from_kmer_to_binary(kmer);
}

