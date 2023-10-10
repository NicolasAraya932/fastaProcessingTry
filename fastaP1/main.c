#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "file_p.h"
#include "kmers.h"
#include "hash.h"
#include "test.h"

pthread_mutex_t buffer_m = PTHREAD_MUTEX_INITIALIZER;

int power(int b, int k)
{
  if(k <= 0) return 1;
  else return b*power(b,k-1);
}

int main(int argc, char **argv)
{
  /* Sync */
  char *sequence = file_char_read(argv[1]);

  int k = atoi(argv[2]);

  /*
      The No. is based on binary representantion
      for each kmer char
  */

  int file_size = get_size();

  int hash_size = power(4, k);

  char *bufferHash = malloc(hash_size + 1 * sizeof(char));

  if(bufferHash == NULL)
  {
    errno = EAGAIN;
    perror("Hash Buffer is NULL");
    exit(1);
  }
  /* endpoint Sync */

  /* Sequences */
  int kmers_size = file_size - k + 1;
  char *kmer = malloc((k+1) * sizeof(char));
  if (kmer == NULL) {
      errno = EAGAIN;
      perror("arr Failed to init the buffer for kmer");
      exit(1);
    }

  /* Save the kmer inside the bufferHash */
  for (int i = 0; i < kmers_size; i++) 
  {
    kmer[k] = '\0';
    strncpy(kmer, sequence + i, k);

    bufferHash[hash_function(kmer)]++;
  }

  int hist[100] = {0};

  for(int i = 0 ; i < hash_size - 1 ; i++)
  {
    int current_hash = bufferHash[i];

    if(current_hash > 100) hist[99]++;
    else if(current_hash > 0) hist[current_hash]++;  
  }

  /* Getting Results */
  for(int i = 1 ; i < 100 ; i++)
  {
    printf("%d %d\n", i, hist[i]);
  }

  free(sequence);
  free(bufferHash);
  free(kmer);

  return 0;
}
