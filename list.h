#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node
{
  char *key;
  char *val;
  struct node *next;
} Node;

typedef Node *Node_ptr;

typedef struct
{
  Node_ptr head;
  Node_ptr last;
} List;

typedef List *List_ptr;

List_ptr create_list(void);
Node_ptr create_node(char *, char *);
void add(List_ptr, char *, char *);
char *get_value(List_ptr, char *);
void print_list(List_ptr list);
void print_matching_key(List_ptr list, char * key);

#endif
