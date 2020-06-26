#ifndef __SHELL_H__
#define __SHELL_H__

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

typedef struct{
  int *fd;
  int dup_0;
  int dup_1;
  int c_0;
  int c_1;
} Fd_Set;

void delegate_to_child(char **args, List_ptr aliases, List_ptr vars, int *fd, int *fd_set, int *code);
void execute_command(char *command, List_ptr aliases, List_ptr vars, int *fd, int *fd_set, int *code);
void load_shrc(List_ptr aliases, List_ptr vars, int *exit_code);
void execute(char *command, List_ptr aliases, List_ptr vars, int *exit_code);

#endif
