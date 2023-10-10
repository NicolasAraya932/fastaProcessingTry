#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int _file_size = 0;
int _extra_chars = 0;

int get_size()
{
  pthread_mutex_lock(&lock);
  return _file_size;
  pthread_mutex_unlock(&lock);
}

void get_file_size(char *_file)
{
  FILE *file = fopen(_file, "r"); 
  pthread_mutex_lock(&lock);
  int c;

  while((c=fgetc(file)) != EOF)
  {
    _file_size++; 
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
    _extra_chars = _extra_chars + bo;
  }
  _file_size = _file_size - _extra_chars;
  pthread_mutex_unlock(&lock);
  fclose(file);
}


char *file_char_read(char *_file)
{
  //int c; 
  int c2; char *result;

  FILE *file = fopen(_file, "r");

  // Get file size
  get_file_size(_file);

  // Dinamic Var to store sequence
  result = malloc(_file_size * sizeof(char) + 1);
  if(result == NULL || (c2 = fgetc(file)) == EOF)
  {
    errno = EAGAIN;
    perror("RESULT BUFFER RETURNED NULL OR THE FILE IS NULL");
    exit(1);
  }

  int i = 0;
  // Store the sequence without the name
  for(i;i < _file_size + 100;i++)
  {
    int c = fgetc(file);

    if(c == EOF) break;

    if(c == '>'){
      while(c != '\n') c=fgetc(file);
    }
    int bo = (c == 'A') ? 1 :\
             (c == 'T') ? 1 :\
             (c == 'C') ? 1 :\
             (c == 'G') ? 1 : 0;

    if(bo) result[i] = c;
  }

  result[_file_size + 1] = '\0';

  fclose(file);

  return result;
}
