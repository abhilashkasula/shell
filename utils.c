#include "utils.h"

char **split_into_two(char * text, char delimiter)
{
  char **split_string = malloc(sizeof(char *) * 2);
  int length = strlen(text);

  for(int i = 0; i < length; i++)
  {
    if(text[i] == delimiter) {
      split_string[0] = malloc(sizeof(char) * i + 1);
      split_string[1] = malloc(sizeof(char) * (length - i));
      split_string[0][i] = '\0';
      memcpy(split_string[0], text, i);
      memcpy(split_string[1], text + i + 1, length - i);
    }
  }

  return split_string;
}

int includes(char *text, char delimiter)
{
  int is_found = 0;
  for(int i = 0; i < strlen(text); i++)
  {
    if(text[i] == delimiter)
    {
      is_found = 1;
    }
  }
  return is_found;
}
