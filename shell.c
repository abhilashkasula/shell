#include "shell.h"

void delegate_to_child(char **args, List_ptr aliases, List_ptr vars, int *fd, int *fd_set, int *code)
{
  int pid = fork();

  if (pid == 0)
  {
    signal(SIGINT, NULL);
    fd_set[2] && close(fd[0]);
    fd_set[3] && close(fd[1]);
    fd_set[0] && dup2(fd[0], 0);
    fd_set[1] && dup2(fd[1], 1);
    handle_redirection(args);
    execvp(args[0], args);
    printf("Command not found\n");
    exit(127);
  }
  else
  {
    waitpid(pid, code, 0);
  }
}

void execute_command(char *command, List_ptr aliases, List_ptr vars, int *fd, int *fd_set, int *code)
{
  char **args = split(command, ' ');
  interpolate(args, vars);

  if (handle_built_in(args, aliases, vars, code))
  {
    return;
  }

  args[0] = get_actual(aliases, args[0]);
  delegate_to_child(args, aliases, vars, fd, fd_set, code);
}

void load_shrc(List_ptr aliases, List_ptr vars, int *exit_code)
{
  char command[255];
  FILE *fptr = fopen(".shrc", "r");

  while (fptr != NULL && fgets(command, 255, fptr) != NULL)
  {
    int len = strlen(command);
    command[len - 1] = command[len - 1] == '\n' ? ' ' : command[len - 1];
    int fd_set[4] = {0, 0, 0, 0};
    execute_command(command, aliases, vars, NULL, fd_set, exit_code);
  }
}

void execute(char *command, List_ptr aliases, List_ptr vars, int *exit_code)
{
  int p[2];
  int fd_set[4] = {0, 0, 0, 0};
  if (!includes(command, '|'))
  {
    execute_command(command, aliases, vars, p, fd_set, exit_code);
    return;
  }
  char **instructions = split(command, '|');
  pipe(p);
  fd_set[1] = 1;
  fd_set[2] = 1;
  execute_command(instructions[0], aliases, vars, p, fd_set, exit_code);
  close(p[1]);
  fd_set[0] = 1;
  fd_set[1] = 0;
  fd_set[2] = 0;
  fd_set[3] = 1;
  execute_command(instructions[1], aliases, vars, p, fd_set, exit_code);
  close(p[0]);
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