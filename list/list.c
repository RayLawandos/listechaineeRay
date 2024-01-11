/*
 * list.c
 *
 * Operation de base de manipulation d'une liste chainee
 */

#include <stdio.h>
#include <stdlib.h>
#include "list/list.h"

/* Extern decl of current listechainee */
extern listechainee_ptr curlist;

void
free_list(listechainee_ptr list)
{
  while(list)
    {
      listechainee_ptr tmp = list;
      list = list->next;
      free(tmp);
    }
}

void
init_list(void)
{
  if (curlist != NULL)
    free_list(curlist);
  curlist = NULL;
}

void
display_list(listechainee_ptr list)
{
  if (list == NULL)
    fprintf(stdout, "Liste vide\n");
  else
    {
      fprintf(stdout, "Liste: ");
      while(list)
        {
          fprintf(stdout, "%d", list->N);
          if (list->next)
            fprintf(stdout, ",");
          list = list->next;
        }
      fprintf(stdout, "\n");
    }
}

listechainee_ptr
reinit_list(listechainee_ptr list)
{
  
}

listechainee_ptr
load_list(char* filename)
{
  
}

void
save_list(char* filename)
{
  
}

int
test_elem_in_list(listechainee_ptr list, int n)
{
  listechainee_ptr tmp = list;
  if (tmp == NULL)
    return 0;
  while(tmp)
    {
      if (tmp->N == n)
        return 1;
      tmp = tmp->next;
    }
  return 0;
}

int
test_ix_in_list(listechainee_ptr list, int ix)
{
  int lix = 0;
  listechainee_ptr tmp = list;
  if (tmp == NULL)
    return 0;
  while(tmp)
    {
      if (lix == ix)
        return 1;
      tmp = tmp->next;
      lix++;
    }
  return 0;
}

listechainee_ptr
append_list(listechainee_ptr list, int n)
{
  if (test_elem_in_list(list, n))
    return NULL;
  
  listechainee_ptr new_node = (listechainee_ptr)malloc(sizeof(struct listechainee_st));
  new_node->N = n;
  new_node->next = (listechainee_ptr)NULL;
  if (list == NULL)
    {
      list = new_node;
    }
  else
    {
      listechainee_ptr tmp = list;
      while(tmp)
        {
          if (tmp->next == NULL)
            break;
          tmp = tmp->next;
        }
      tmp->next = new_node;
    }
  return list;  
}

listechainee_ptr 
prepend_list(listechainee_ptr list, int n)
{
  if (test_elem_in_list(list, n))
    return NULL;
  
  listechainee_ptr new_node = (listechainee_ptr)malloc(sizeof(struct listechainee_st));
  new_node->N = n;
  new_node->next = list;
  list = new_node;

  return list;
}

listechainee_ptr
insert_elem_in_list(listechainee_ptr list, int ix, int n)
{
  if (test_ix_in_list(list, ix))
    {
      int lix = 0;
      listechainee_ptr tmp = list;
      while(tmp)
        {
          if (lix == ix)
            {
              listechainee_ptr newn = (listechainee_ptr)malloc(sizeof(struct listechainee_st));
              if (newn == NULL)
                return NULL;
              newn->N = n;
              if (ix == 0)
                {
                  newn->next = tmp->next;
                  return newn;
                }
              else
                {
                  newn->next = tmp->next;
                  tmp->next = newn;
                  return list;                  
                }
            }
          tmp = tmp->next;
          lix++;
        }      
    }
  else
    return(NULL);
}

void
delete_end_of_list(listechainee_ptr list)
{
  
}

listechainee_ptr
delete_start_of_list(listechainee_ptr list)
{
  
} 

listechainee_ptr
delete_elem_in_list(listechainee_ptr list, int ix)
{
  
}

