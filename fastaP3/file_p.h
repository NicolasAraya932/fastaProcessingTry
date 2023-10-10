
typedef struct buffer{
  char **buf;
  int start;
  int end;
} buffer;

typedef struct node{
  int start;
  int end;
  struct node *next;
} node;

char *delInvalidChar(char *str);

char **file_char_read(char *_file);

int get_file_size(char *_file);

