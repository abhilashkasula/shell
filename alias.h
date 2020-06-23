#ifndef __ALIAS_H__
#define __ALIAS_H__

#include "list.h"
void handle_alias(char **argv, List_ptr aliases);
char *get_actual(List_ptr aliases, char *aka);

#endif