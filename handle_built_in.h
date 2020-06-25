#ifndef __HANDLE_BUILT_IN__
#define __HANDLE_BUILT_IN__

#include <unistd.h>
#include <fcntl.h>
#include "alias.h"
#include "var.h"

void open_file_and_duplicate(char **args, int index, int mode);
void handle_output_redirection(char **args, int index);
void handle_redirection(char **args);
int handle_built_in(char **args, List_ptr aliases, List_ptr vars, int *exit_code);

#endif
