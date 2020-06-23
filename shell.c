#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include "alias.h"
#include "utils.h"

#define COLOR "\e[0;36m"
#define RESET "\e[0m"

void exit_process(int sig)
{
  exit(0);
}

void parse(char *command, char **argv)
{
  while (*command != '\0')
  {
    if (*command == ' ' || *command == '\t' || *command == '\n')
    {
      *command = '\0';
      command++;
    }
    *argv = command;
    argv++;

    while (*command != '\0' && *command != ' ' && *command != '\t' && *command != '\n')
    {
      command++;
    }
  }
  *argv = NULL;
}

int is_handled(char **argv, List_ptr alias_list)
{
  char *actual = get_actual(alias_list, argv[0]);
  if (strcmp(actual, "exit") == 0)
  {
    exit_process(1);
  }
  if (strcmp(actual, "cd") == 0)
  {
    chdir(argv[1]);
    return 1;
  }
  if (strcmp(actual, "alias") == 0)
  {
    handle_alias(argv, alias_list);
    return 1;
  }
  return 0;
}

int main(void)
{
  char command[255];
  char *argv[255];
  char pwd[255];
  char dollar_color[10] = "\e[0;32m";
  signal(SIGINT, SIG_IGN);
  List_ptr alias_list = create_list();
  while (1)
  {
    getcwd(pwd, sizeof(pwd));
    printf(COLOR "\n %s " RESET, pwd);
    printf("%s$ " RESET, dollar_color);
    gets(command);
    parse(command, argv);
    if (is_handled(argv, alias_list))
    {
      continue;
    }

    char *actual = get_actual(alias_list, argv[0]);
    int pid = fork();

    if (pid == 0)
    {
      signal(SIGINT, exit_process);
      execvp(actual, argv);
      printf("Command not found\n");
      exit(1);
    }
    else
    {
      wait(&pid);
      dollar_color[5] = pid ? '1' : '2';
    }
  }
  return 0;
}