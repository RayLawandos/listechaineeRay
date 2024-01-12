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

/*
 * free_list
 *
 * Take a list as input and free all nodes in it
 * params:
 *   list - in: list to free
 * returns:
 *   void
 */
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

/*
 * init_list
 *
 * Initialize the current list as a NULL list. If curlist
 * is not null, free it.
 * params:
 *   void
 * returns:
 *   void
 */
void
init_list(void)
{
  if (curlist != NULL)
    free_list(curlist);
  curlist = NULL;
}

/*
 * display_list
 *
 * Take a list as input and displays all nodes in it. If list
 * is null, displays "Empty list".
 * params:
 *   list - in: the list to display
 * returns:
 *   void
 */
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

/*
 * test_elem_in_list
 *
 * Take a list and an elem as input, and returns if this elem is
 * contained in this list.
 * params:
 *   list - in: the list in which to test if elem is in
 *   n    - in: the element to test
 * returns:
 *   0: if elem is not in the list
 *   1: if elem is in the list
 */
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

/*
 * test_ix_in_list
 *
 * Take a list and an index as input, and returns if this index is
 * contained in this list (number of elem in list > ix).
 * params:
 *   list - in: the list in which to test if elem is in
 *   ix   - in: the element to test
 * returns:
 *   0: if ix is not in the list
 *   1: if ix is in the list or ix == 0 & list is NULL
 */
int
test_ix_in_list(listechainee_ptr list, int ix)
{
  /* If ix not positive or null */
  if (ix < 0)
    /* returns No */
    return 0;
  /* Local index */
  int lix = 1;
  /* Temp node ptr */
  listechainee_ptr tmp = list;
  /* Browse the list */
  while(tmp)
    {
      /* If index is found */
      if (lix == ix)
        /* Return Yes */
        return 1;
      /* Goes on next node */
      tmp = tmp->next;
      /* Incr local index (next one) */
      lix++;
    }
  /* If index was null it is always available else return No */
  return ix == 0 ? 1 : 0;
}

/*
 * append_list
 *
 * Take a list and element as input and append the element
 * at the end of the list.
 * params:
 *   list - in: list in which to append the new element
 *   n    - in: element to append to the list
 * returns:
 *   the list
 */
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

/*
 * prepend_list
 *
 * Take a list and element as input and prepend the element
 * at the start of the list.
 * params:
 *   list - in: list in which to prepend the new element
 *   n    - in: element to prepend to the list
 * returns:
 *   the list
 */
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

/*
 * insert_elem_in_list
 *
 * Take a list, an element and an index as input and insert the element 'n'
 * in the list 'list' at the index 'ix'. if index is not in list or if
 * element is already in list, returns NULL.
 * params:
 *   list - in: the list in which to insert the element
 *   ix   - in: the index at which the element has to be inserted
 *   n    - in: the element to insert in the list at the index
 * returns:
 *   the list if the element is not in the list and was inserted
 *   NULL otherwhise
 */
listechainee_ptr
insert_elem_in_list(listechainee_ptr list, int ix, int n)
{
  /* First test pre conditions: ix in list and elem not in list */
  if (test_ix_in_list(list, ix) && !test_elem_in_list(list, n))
    {
      /* local ix */
      int lix = 0;
      /* tmp node ptr */
      listechainee_ptr tmp = list;
      /* while tmp not null */
      while(tmp)
        {
          /* If index was found */
          if (lix == ix)
            {
              /* Allocate new node */
              listechainee_ptr newn = (listechainee_ptr)malloc(sizeof(struct listechainee_st));
              if (newn == NULL)
                /* Mem alloc pb */
                return NULL;
              /* Init node */
              newn->N = n;
              /* If index is 0, insert at start of list */
              if (ix == 0)
                {
                  /* New node is the new list */
                  newn->next = tmp->next;
                  /* Then returns it */
                  return newn;
                }
              /* Insert in the list (after the first element) */
              else
                {
                  /* Link new node to next node */
                  newn->next = tmp->next;
                  /* Insert new node */
                  tmp->next = newn;
                  /* Returns list */
                  return list;                  
                }
            }
          /* Goes on to next node */
          tmp = tmp->next;
          /* And incr local ix */
          lix++;
        }      
    }
  else
    /* returns NULL: cannot insert */
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

