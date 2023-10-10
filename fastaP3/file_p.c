#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include <omp.h>

struct node {
  int start;
  int end;
  struct node *next;
};

struct buffer{
  char **buf;
  int start;
  int end;
};

int first_line = 0;

char *delInvalidChar(char *str) {
    int size = strlen(str);
    char *result = malloc((size + 1) * sizeof(char)); // Allocate space for the result string

    if (result == NULL) {
        // Handle memory allocation error
        return NULL;
    }

    int j = 0; // Index for the result string

    for (int i = 0; i < size; i++) {
        switch (str[i]) {
            case 'A':
            case 'T':
            case 'C':
            case 'G':
                result[j++] = str[i];
                break;
            default:
                break;
        }
    }

    result[j] = '\0'; // Null-terminate the result string

    return result;
}

int get_file_size(char *_file)
{

  double start_time = omp_get_wtime();

  FILE *file = fopen(_file, "r"); 
  int c;
  int size = 0;
  int estado = 1;

  while((c=fgetc(file)) != EOF)
  {
    if(c == 'A' || c == 'T' || c == 'C' || c == 'G')
    {
      size++; 
    }
  }

  fclose(file);
  double end_time = omp_get_wtime();

  printf("TIME TO GET FILE SIZE %f\n", end_time - start_time);

  return size;
}


char **file_char_read(char *_file)
{
  int c; int c2;

  FILE *file = fopen(_file, "r");

  // Get file size
  int file_size = get_file_size(_file);

  printf("%d\n", file_size);
  int string_s4 = file_size / 4;

  double start_time = omp_get_wtime();

  // Dinamic Var to store sequence
  char *string = malloc(file_size * sizeof(char));  
  char **result = malloc(4);

  if(result == NULL || (c2 = fgetc(file)) == EOF)
  {
    errno = EAGAIN;
    perror("RESULT BUFFER RETURNED NULL OR THE FILE IS NULL");
    exit(1);
  }

  /* Chance to corrupt involved variables if you only use string[file_size] bc is a fixed array. Use
   * Dinamic only for fread
   */
  fread(string, 1, file_size, file);

  int counter = 0;

  omp_set_num_threads(4);

  #pragma omp parallel for 
  for(int i = 0 ; i < 4 ; i++)
  {
    int start = i * string_s4;
    int end = ((i+1) * string_s4 - 1);

    int segment_size = end - start;

    printf("start %d\t end %d\t total_current_size: %d\t thread_buffer_size: %d\t string_size: %d\t \n",start, end, counter, end - start + 1, file_size);

    result[i] = malloc((segment_size+1) * sizeof(char));
    if(result[i] == NULL)
    {
      errno = EAGAIN;
      perror("RESULT[i] BUFFER RETURNED NULL OR THE FILE IS NULL");
      exit(1);
    }

    result[i][segment_size] = '\0';
    strncpy(result[i], string + start, segment_size+ 1);

    printf("HECHO en i %d\n", i);
  }

  double end_time = omp_get_wtime();

  printf("TIME FOR FILE READ %f\n", end_time - start_time);

  free(string);

  return result;
}
