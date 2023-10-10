
typedef struct buffer{
  char **buf;
  int start;
  int end;
} buffer;

char *file_char_read(char *_file);

int get_file_size(char *_file);
