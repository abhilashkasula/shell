#include "list.h"

List_ptr create_list(void)
{
  List_ptr list = malloc(sizeof(List));
  list->head = NULL;
  list->last = NULL;
  return list;
}

Node_ptr create_node(char * key, char * val)
{
  Node_ptr node = malloc(sizeof(Node));
  node->key = key;
  node->val = val;
  node->next = NULL;
  return node;
}

void add(List_ptr list, char *key, char * value)
{
  Node_ptr *p_walk = &list->head;

  while (*p_walk != NULL)
  {
    p_walk = &((*p_walk)->next);
  }

  (*p_walk) = create_node(key, value);
  list->last = *p_walk;
}

char * get_value(List_ptr list, char *key)
{

  Node_ptr p_walk = list->head;
  int is_found = 0;
  char *val = NULL;

  while (!is_found && p_walk != NULL)
  {
    if(strcmp(p_walk->key, key) == 0)
    {
      val = malloc(sizeof(char) * strlen(p_walk->val));
      val = p_walk->val;
      is_found = 1;
    }

    p_walk = p_walk->next;
  }

  return val;
}

void print_list(List_ptr list)
{
  Node_ptr p_walk = list->head;

  while (p_walk != NULL)
  {
    printf("%s=%s\n", p_walk->key, p_walk->val);
    p_walk = p_walk->next;
  }
}

void print_matching_key(List_ptr list, char * key)
{
  if(!key)
  {
    print_list(list);
    return;
  }
  
  Node_ptr p_walk = list->head;

  while (p_walk != NULL)
  {
    if(strcmp(p_walk->key, key) == 0)
    {
      printf("%s=%s\n", p_walk->key, p_walk->val);
    }
    p_walk = p_walk->next;
  }
}
