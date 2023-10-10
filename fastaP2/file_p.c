#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <omp.h>

struct buffer{
  char **buf;
  int start;
  int end;
};

int get_file_size(char *_file)
{

  double start_time = omp_get_wtime();

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
  double end_time = omp_get_wtime();

  printf("TIME TO GET FILE SIZE %f\n", end_time - start_time);

  return file_size;
}


char *file_char_read(char *_file)
{
  int c; int c2; char *result;

  FILE *file = fopen(_file, "r");

  // Get file size
  int file_size = get_file_size(_file);

  double start_time = omp_get_wtime();
  // Dinamic Var to store sequence
  result = malloc(file_size * sizeof(char) + 1);
  if(result == NULL || (c2 = fgetc(file)) == EOF)
  {
    errno = EAGAIN;
    perror("RESULT BUFFER RETURNED NULL OR THE FILE IS NULL");
    exit(1);
  }

  int i = 0;
  // Store the sequence without the name
  while((c=fgetc(file)) != EOF)
  {
    if(c == '>'){
      while(c != '\n') c=fgetc(file);
    }
    int bo = (c == 'A') ? 1 :\
             (c == 'T') ? 1 :\
             (c == 'C') ? 1 :\
             (c == 'G') ? 1 : 0;

    if(bo) result[i++] = c;
  }

  result[file_size + 1] = '\0';

  fclose(file);

  double end_time = omp_get_wtime();

  printf("TIME FOR FILE READ %f\n", end_time - start_time);

  return result;
}
