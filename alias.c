#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "alias.h"
#include "utils.h"

void add_alias(List_ptr aliases, char *alias)
{
  char **split_alias = split(alias, '=');
  add(aliases, split_alias[0], split_alias[1]);
}

void handle_alias(char **argv, List_ptr aliases)
{
  if(argv[1] && argv[1][0] != '=' && includes(argv[1], '='))
  {
    add_alias(aliases, argv[1]);
    return;
  }
  print_matching_key(aliases, argv[1]);
}

char *get_actual(List_ptr aliases, char *aka)
{
  char *actual = get_value(aliases, aka);
  return actual ? actual : aka;
}
