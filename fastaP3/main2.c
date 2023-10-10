#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include <omp.h>

#include "file_p.h"
#include "kmers.h"
#include "hash.h"
#include "test.h"


int power(int b, int k)
{
  if(k <= 0) return 1;
  else return b*power(b,k-1);
}


int main(int argc, char **argv)
{
  int max_threads = omp_get_max_threads();
  omp_set_num_threads(max_threads);

  double start_total_time = omp_get_wtime();

  int k = atoi(argv[2]);

  printf("BEGIN FILE READ\n");
  char **sequence = file_char_read(argv[1]);

  #pragma omp parallel for
  for(int i = 0;i < 4;i++)
  {
    sequence[i] = delInvalidChar(sequence[i]);
  }

  int hash_size = power(4, k);

  char *bufferHash = malloc(hash_size + 1 * sizeof(char));

  if(bufferHash == NULL)
  {
    errno = EAGAIN;
    perror("Hash Buffer is NULL");
    exit(1);
  }

  char ***allSeq = malloc(4 * sizeof(char **));
  printf("SAVING KMERS TO BUFFER\n");

  double start_kmer = omp_get_wtime();

  #pragma omp parallel for
  for(int i = 0;i<4;i++)
  {
  printf("IM HERE 0\n");
    long int segment_size = strlen(sequence[i]);

    long int segment_1000 = segment_size / 1000;


    for(int j = 0; j < 1000 ; j++)
    {
      int start = j * segment_1000;
      int end = (j + 1) * segment_1000 - 1;
    }

    /*
    long int num_kmers = segment_size - k + 1;

    printf("%lu\n", num_kmers);

    allSeq[i] = malloc(num_kmers + sizeof(char *));
    if(allSeq[i] == NULL)
    {
      errno = EAGAIN;
      perror("allSeq[i] Buffer is NULL");
      exit(1);
    }

    for(int j = 0; j < num_kmers; j++)
    {
      printf("IM HERE 1");
      allSeq[i][j] = malloc((k+1)*sizeof(char));

      allSeq[i][j][k] = '\0';
      strncpy(allSeq[i][j], sequence[i] + j, k);
      

      if(strlen(allSeq[i][j]) == k)
      {
        bufferHash[hash_function(allSeq[i][j])]++;
      }
      else
      {
        printf("DIFF %lu\t k %d\t strlen %lu\n", k - strlen(allSeq[i][j]), k, strlen(allSeq[i][j]));
      }
    }
  }
  */
  double end_kmer = omp_get_wtime() - start_kmer;

  
  int hist[101] = {0};

  for(int i = 0 ; i < hash_size - 1 ; i++)
  {
    int current_hash = bufferHash[i];

    if(current_hash > 100) hist[100]++;
    else if(current_hash > 0) hist[current_hash]++;  
  }

  for(int i = 1 ; i < 101 ; i++)
  {
    printf("%d %d\n", i, hist[i]);
  }

  free(sequence);
  free(bufferHash);
  free(allSeq);

  double end_total_time = omp_get_wtime();

  printf("TOTAL TIME TAKEN %f s\t NUM THREADS %d\n", end_total_time - start_total_time, omp_get_max_threads());
  return 0;
}
