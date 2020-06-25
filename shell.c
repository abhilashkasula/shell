#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "alias.h"
#include "utils.h"
#include "var.h"

#define COLOR "\e[0;36m"
#define GREEN "\e[0;32m"
#define RED "\e[0;31m"
#define RESET "\e[0m"

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

void handle_redirection(char **args)
{
  int index = includes_array(args, '>');
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

int is_handled(char **args, List_ptr aliases, List_ptr vars, int *exit_code)
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

void execute(char *command, List_ptr aliases, List_ptr vars, int *exit_code)
{
  char **args = split(command, ' ');
  interpolate(args, vars);

  if (is_handled(args, aliases, vars, exit_code))
  {
    return;
  }

  char *actual = get_actual(aliases, args[0]);
  int pid = fork();

  if (pid == 0)
  {
    signal(SIGINT, NULL);
    handle_redirection(args);
    execvp(actual, args);
    printf("Command not found\n");
    exit(127);
  }
  else
  {
    waitpid(pid, exit_code, 0);
  }
}

void load_shrc(List_ptr aliases, List_ptr vars, int *exit_code)
{
  char command[255];
  FILE *fptr = fopen(".shrc", "r");

  while (fptr != NULL && fgets(command, 255, fptr) != NULL)
  {
    int len = strlen(command);
    command[len - 1] = command[len - 1] == '\n' ? ' ' : command[len - 1];
    execute(command, aliases, vars, exit_code);
  }
}

int main(void)
{
  char command[255];
  char pwd[255];
  signal(SIGINT, SIG_IGN);
  List_ptr aliases = create_list();
  List_ptr vars = create_list();
  int exit_code = 0;
  load_shrc(aliases, vars, &exit_code);
  while (1)
  {
    getcwd(pwd, sizeof(pwd));
    printf(COLOR "\n %s " RESET, pwd);
    printf(exit_code ? RED : GREEN);
    printf("$ " RESET);
    gets(command);
    execute(command, aliases, vars, &exit_code);
  }
  return 0;
}