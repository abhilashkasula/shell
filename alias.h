#ifndef __ALIAS_H__
#define __ALIAS_H__

#include "list.h"
// typedef struct alias
// {
//   char *key;
//   char *val;
//   struct alias *next;
// } Alias;

// typedef Alias *Alias_ptr;

// typedef struct
// {
//   Alias_ptr first;
//   Alias_ptr last;
// } Alias_List;

// typedef Alias_List *Alias_List_ptr;

// Alias_List_ptr create_alias_list(void);
void handle_alias(char **argv, List_ptr aliases);
char *get_actual(List_ptr aliases, char *aka);

#endif