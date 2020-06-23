#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "alias.h"

char **split_into_two(char * alias, char delimiter)
{
  char **split_string = malloc(sizeof(char *) * 2);
  int length = strlen(alias);

  for(int i = 0; i < length; i++)
  {
    if(alias[i] == delimiter) {
      split_string[0] = malloc(sizeof(char) * i + 1);
      split_string[1] = malloc(sizeof(char) * (length - i));
      split_string[0][i] = '\0';
      memcpy(split_string[0], alias, i);
      memcpy(split_string[1], alias + i + 1, length - i);
    }
  }

  return split_string;
}

Alias_ptr create_alias(char *key, char *val)
{
  Alias_ptr alias = malloc(sizeof(Alias));
  alias->key = key;
  alias->val = val;
  alias->next = NULL;
  return alias;
}

void add_alias(Alias_List_ptr aliases, char *alias)
{
  char **split_alias = split_into_two(alias, '=');
  Alias_ptr *p_walk = &aliases->first;

  while (*p_walk != NULL)
  {
    p_walk = &((*p_walk)->next);
  }

  (*p_walk) = create_alias(split_alias[0], split_alias[1]);
  aliases->last = *(p_walk);
}

Alias_List_ptr create_alias_list(void)
{
  Alias_List_ptr list = malloc(sizeof(Alias_List));
  list->first = NULL;
  list->last = NULL;
  return list;
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

void print_match_aliases(Alias_List_ptr aliases, char *match)
{
  Alias_ptr p_walk = aliases->first;

  while (p_walk != NULL)
  {
    char * text_to_match = match == NULL ? p_walk->key : match;
    if(strcmp(p_walk->key, text_to_match) == 0)
    {
      printf("%s=%s\n", p_walk->key, p_walk->val);
    }
    p_walk = p_walk->next;
  }
}

void handle_alias(char **argv, Alias_List_ptr aliases)
{
  if(argv[1] && argv[1][0] != '=' && includes(argv[1], '='))
  {
    add_alias(aliases, argv[1]);
    return;
  }
  print_match_aliases(aliases, argv[1]);
}

char *get_actual(Alias_List_ptr aliases, char *aka)
{
  Alias_ptr p_walk = aliases->first;

  while (p_walk != NULL)
  {
    if(strcmp(p_walk->key, aka) == 0)
    {
      aka = p_walk->val;
    }
    p_walk = p_walk->next;
  }
  
  return aka;
}
