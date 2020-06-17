#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void exit_process(int sig)
{
  exit(0);
}

void parse(char *command, char **argv)
{
  while (*command != '\0')
  {
    if(*command == ' ' || *command == '\t' || *command == '\n')
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
  *argv = 0;
}

int main(void) {
  char command[255];
  char *argv[255];
  while (1)
  {
    printf("my shell $ ");
    signal(SIGINT, SIG_IGN);
    gets(command);
    parse(command, argv);
    int pid = fork();
    if(pid == 0)
    {
      signal(SIGINT, exit_process);
      execvp(*argv, argv);
    }
    else
    {
      wait(&pid);
    }
  }
  return 0;
}