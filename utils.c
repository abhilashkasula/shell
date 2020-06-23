#include "utils.h"

char **split(char *string, char delimiter)
{
  char *args[10];
  int length = strlen(string);
  char text[length];
  int index = 0;
  int args_length = 0;

  for (int i = 0; i <= length; i++)
  {
    if (string[i] == delimiter || string[i] == 0)
    {
      text[index] = '\0';
      index++;
      args[args_length] = malloc(sizeof(char) * index);
      memcpy(args[args_length], text, index);
      index = 0;
      args_length++;
    }
    else
    {
      text[index] = string[i];
      index++;
    }
  }

  args[args_length] = NULL;
  char **parsed = malloc(sizeof(char *) * args_length + 1);
  memcpy(parsed, args, sizeof(char *) * args_length + 1);
  return parsed;
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
