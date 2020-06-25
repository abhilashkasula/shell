#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include "alias.h"
#include "utils.h"
#include "var.h"
#include "handle_built_in.h"

#define COLOR "\e[0;36m"
#define GREEN "\e[0;32m"
#define RED "\e[0;31m"
#define RESET "\e[0m"

void execute(char *command, List_ptr aliases, List_ptr vars, int *exit_code)
{
  char **args = split(command, ' ');
  interpolate(args, vars);

  if (handle_built_in(args, aliases, vars, exit_code))
  {
    return;
  }

  char *actual = get_actual(aliases, args[0]);
  int pid = fork();

  if (pid == 0)
  {
    signal(SIGINT, NULL);
    printf("%s", actual);
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