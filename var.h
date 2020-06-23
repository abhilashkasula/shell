#ifndef __VAR_H__
#define __VAR_H__

#include "list.h"
#include "utils.h"

int add_var(List_ptr vars, char *var);
void interpolate(char **args, List_ptr vars);
char *get_var_val(List_ptr vars, char *key);

#endif
