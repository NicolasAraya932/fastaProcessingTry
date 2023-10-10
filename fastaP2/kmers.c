#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*
procedure k-mers(string seq, integer k) is
	L ← length(seq)
	arr ← new array of L − k + 1 empty strings

	// iterate over the number of k-mers in seq,
	// storing the nth k-mer in the output array
	for n ← 0 to L − k + 1 exclusive do
			arr[n] ← subsequence of seq from letter n inclusive to letter n + k exclusive

	return arr
*/

char **k_mers(char *seq, int k, int L) 
{
  int num_kmers = L - k + 1;

  char **arr = malloc(num_kmers * sizeof(char *));
  if (arr == NULL) {
    errno = EAGAIN;
    perror("**arr Failed to init the buffer for kmer in char **k_mers(const char *seq, int k, long int L)");
    exit(1);
  }

  for (int i = 0; i < num_kmers; i++) 
  {
    arr[i] = malloc((k + 1) * sizeof(char));
    if (arr[i] == NULL) {
      errno = EAGAIN;
      perror("arr Failed to init the buffer for kmer in char **k_mers(const char *seq, int k, long int L)");
      exit(1);
    }

    arr[i][k] = '\0';
    strncpy(arr[i], seq + i, k);
  }
  return arr;
}

