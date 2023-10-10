#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include <omp.h>

int get_file_size(char *_file)
{
  FILE *file = fopen(_file, "r"); 
  int c;
  int file_size = 0;

  while((c=fgetc(file)) != EOF)
  {
    file_size++; 
    if(c == '>')
    {
      /* Remove sequence name */
      while(c != '\n')
      {
        c = fgetc(file);
      }
    }
    int bo = (c == 'A') ? 0 :\
             (c == 'T') ? 0 :\
             (c == 'C') ? 0 :\
             (c == 'G') ? 0 : 1;
    file_size = file_size - bo; 
  }
  fclose(file);

  return file_size;
}


char *file_char_read(char *_file)
{
  //int c; 
  int c2; char *result;

  FILE *file = fopen(_file, "r");

  // Get file size
  int file_size = get_file_size(_file);

  // Dinamic Var to store sequence
  result = malloc(file_size * sizeof(char) + 1);
  if(result == NULL || (c2 = fgetc(file)) == EOF)
  {
    errno = EAGAIN;
    perror("RESULT BUFFER RETURNED NULL OR THE FILE IS NULL");
    exit(1);
  }

  // Store the sequence without the name
  #pragma omp parallel for num_threads(10)
  for(int i = 0 ; i < file_size ; i++)
  {
    int x, c = fgetc(file);

    if(c == EOF) i = x;

    if(c == '>') while(c != '\n') c=fgetc(file);

    int bo = (c == 'A') ? 1 :\
             (c == 'T') ? 1 :\
             (c == 'C') ? 1 :\
             (c == 'G') ? 1 : 0;

    if(bo)
    {
      #pragma omp critical
      result[i] = c;
    }
    x = i;
    //printf("i = %d\t result %s\n",i, result);
  }

  result[file_size + 1] = '\0';

  fclose(file);

  return result;
}
