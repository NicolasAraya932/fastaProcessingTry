#include <stdio.h>

int main(int argc, char **argv)
{

  FILE *file = fopen(argv[1], "r");

  int c;
  for(int k = 0 ; k < 20 ; k++)
  {
    printf("%c", (c = fgetc(file) + k));
  }

  return 0;
}
