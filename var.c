#include "var.h"

int add_var(List_ptr vars, char *var)
{
  char **split_vars = split(var, '=');
  add(vars, split_vars[0], split_vars[1]);
  return split_vars[2] ? 1 : 0;
}

char *get_var_val(List_ptr vars, char *key)
{
  char new_key[strlen(key)];
  memcpy(new_key, key + 1, strlen(key));
  char *val = get_value(vars, new_key);
  return val ? val : "";
}

void interpolate(char **args, List_ptr vars)
{
  int i = 0;
  while (args[i])
  {
    if (args[i][0] == '$')
    {
      args[i] = get_var_val(vars, args[i]);
    }
    i++;
  }
}
