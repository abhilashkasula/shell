#include "handle_built_in.h"

void open_file_and_duplicate(char **args, int index, int mode)
{
  int fd = open(args[index + 1], O_RDWR);
  dup2(fd, mode);
  args[index] = NULL;
}

void handle_output_redirection(char **args, int index)
{
  creat(args[index + 1], S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  open_file_and_duplicate(args, index, 1);
}

int is_file_present(char * path)
{
  FILE *fp = fopen(path, "r");
  if(fp)
  {
    fclose(fp);
    return 1;
  }
  return 0;
}

void handle_append(char **args, int index)
{
  printf("in");
  if(!is_file_present(args[index + 1]))
  {
    creat(args[index + 1], S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  }
  int fd = open(args[index + 1], O_WRONLY | O_APPEND);
  dup2(fd, 1);
  args[index] = NULL;
}

void handle_redirection(char **args)
{
  int index = includes_array(args, '>');
  if(index > 0 && strcmp(args[index], ">>") == 0)
  {
    handle_append(args, index);
  }

  index = includes_array(args, '>');
  if (index > 0)
  {
    handle_output_redirection(args, index);
  }
  index = includes_array(args, '<');
  if (index > 0)
  {
    open_file_and_duplicate(args, index, 0);
  }
}

int handle_built_in(char **args, List_ptr aliases, List_ptr vars, int *exit_code)
{
  char *actual = get_actual(aliases, args[0]);
  if (strcmp(actual, "exit") == 0)
  {
    exit(0);
  }
  if (strcmp(actual, "cd") == 0)
  {
    *exit_code = chdir(args[1]);
    return 1;
  }
  if (strcmp(actual, "alias") == 0)
  {
    handle_alias(args, aliases);
    *exit_code = 0;
    return 1;
  }
  if (includes(actual, '='))
  {
    *exit_code = add_var(vars, actual);
    return 1;
  }
  return 0;
}
