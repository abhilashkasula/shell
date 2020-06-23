#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include "alias.h"
#include "utils.h"

#define COLOR "\e[0;36m"
#define GREEN "\e[0;32m"
#define RED "\e[0;31m"
#define RESET "\e[0m"

int is_handled(char **args, List_ptr alias_list, int *exit_code)
{
  char *actual = get_actual(alias_list, args[0]);
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
    handle_alias(args, alias_list);
    return 1;
  }
  return 0;
}

void execute(char *command, List_ptr aliases, int *exit_code)
{
  char **args = split(command, ' ');
  if (is_handled(args, aliases, exit_code))
  {
    return;
  }

  char *actual = get_actual(aliases, args[0]);
  int pid = fork();

  if (pid == 0)
  {
    signal(SIGINT, NULL);
    execvp(actual, args);
    printf("Command not found\n");
    exit(127);
  }
  else
  {
    waitpid(pid, exit_code, 0);
  }
}

int main(void)
{
  char command[255];
  char pwd[255];
  signal(SIGINT, SIG_IGN);
  List_ptr alias_list = create_list();
  int exit_code = 0;
  while (1)
  {
    getcwd(pwd, sizeof(pwd));
    printf(COLOR "\n %s " RESET, pwd);
    printf(exit_code ? RED : GREEN);
    printf("$ " RESET);
    gets(command);
    execute(command, alias_list, &exit_code);
  }
  return 0;
}