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
  double start_total_time = omp_get_wtime();
  /* Sync */
  printf("BEGIN FILE READ\n");
  char *sequence = file_char_read(argv[1]);

  int k = atoi(argv[2]);

  /*
      The No. is based on binary representantion
      for each kmer char
  */

  printf("GET SECOND FILE SIZE FOR MAIN\n");
  int file_size = get_file_size(argv[1]);

  int hash_size = power(4, k);

  char *bufferHash = malloc(hash_size + 1 * sizeof(char));

  if(bufferHash == NULL)
  {
    errno = EAGAIN;
    perror("Hash Buffer is NULL");
    exit(1);
  }

  int allSeq_size = (int)(file_size/1000);

  buffer allSeq[allSeq_size];

  int max_threads = omp_get_max_threads();
  omp_set_num_threads(max_threads * 4);

  /* Sequences */

  printf("SAVING SECUENCES TO BUFFER\n");
  double start_time = omp_get_wtime();

  /* Step 1: create a new team of threads just for the use of for loop.
   * Step 2: Each iteration of the for loop will be assigned a different thread.
   *Once execution of all threads is complete, all threads in the team to merged to the original team that is master thread.
  This directive enables the parallel execution of the for loop only.
  */
  #pragma omp parallel for 
  for(int i = 0; i < allSeq_size; i++)
  {
    allSeq[i].buf = malloc(1000 * sizeof(char *));

    allSeq[i].start = i * 1000;
    allSeq[i].end = (i+1) * 1000 - 1;

    int z = 0;
    for(int j = allSeq[i].start ; j < allSeq[i].end; j++)
    {
      allSeq[i].buf[z] = malloc((k+1) * sizeof(char));

      allSeq[i].buf[z][k] = '\0';
      strncpy(allSeq[i].buf[z], sequence + j, k);

      bufferHash[hash_function(allSeq[i].buf[z])]++;

      z++;
    }
  }
  double end_time = omp_get_wtime();
  double buffer_time = end_time - start_time;
  printf("SECUENCES TO BUFFER TIME %f\n\n", end_time - start_time);
  
   /* Free the memory allocated for the buffers. */
  for (int i = 0; i < allSeq_size; i++) {
    for (int j = 0; j < 1000; j++) {
      free(allSeq[i].buf[j]);
    }
    free(allSeq[i].buf);
  }

  /* Histogram */
  int hist[101] = {0};

  for(int i = 0 ; i < hash_size - 1 ; i++)
  {
    int current_hash = bufferHash[i];

    if(current_hash > 100) hist[100]++;
    else if(current_hash > 0) hist[current_hash]++;  
  }

  /* Getting Results */
  for(int i = 1 ; i < 101 ; i++)
  {
    printf("%d %d\n", i, hist[i]);
  }

  free(sequence);
  free(bufferHash);

  double end_total_time = omp_get_wtime();

  printf("TOTAL TIME TAKEN %f\t NUM THREADS %d\n", end_total_time - start_total_time, omp_get_max_threads());
  return 0;
}
