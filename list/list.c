/*
 * list.c
 *
 * Operation de base de manipulation d'une liste chainee
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "list/list.h"

// Buffer size for loading/saving list files
#define BUFFER_MAX_SIZE 		  20

/* Extern decl of current listechainee */
extern listechainee_ptr curlist;
/* Extern decl of program name */
extern char* progname;

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
  /* Browse all list nodes */
  while(list)
    {
      /* Get a tmp ptr for the current node */
      listechainee_ptr tmp = list;
      /* Move head of the list to the next node */
      list = list->next;
      /* Free the current node */
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
  /* If current list not empty */
  if (curlist != NULL)
    /* Free the list */
    free_list(curlist);
  /* Set current list as empty */
  curlist = NULL;
}

/*
 * reinit_list
 *
 * Re-initialize the current list as the list provided. If curlist
 * is not null, free it.
 * params:
 *   void
 * returns:
 *   void
 */
listechainee_ptr
reinit_list(listechainee_ptr list)
{
  /* Init the list to an empty one */
  init_list();
  /* set current list and returns it */
  return (curlist = list);
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
  /* If list is empty */
  if (list == (listechainee_ptr)NULL)
    /* Displays it */
    fprintf(stdout, "Empty list\n");
  else
    {
      /* Print the list */
      fprintf(stdout, "List: [");
      /* For each list element */
      while(list)
        {
          /* print element */
          fprintf(stdout, "%d", list->N);
          /* If not at end of list */
          if (list->next)
            /* Displays the comma */
            fprintf(stdout, ",");
          /* Switch to the next node */
          list = list->next;
        }
      /* Close the display of the list */
      fprintf(stdout, "]\n");
    }
}

/*
 * load_list
 *
 * Load a linked list from the file named <filename>.
 * If loading fails, NULL is returned.
 * params:
 *   filename - in: the name of the file to load the list from
 * returns:
 *   the loaded list if successfull
 *   NULL otherwise
 */
listechainee_ptr
load_list(char* filename)
{
  /* Reinit the current list */
  listechainee_ptr list = reinit_list(curlist);
  /* Open the file 'filename' for reading */
  FILE *fp = fopen(filename, "r");
  /* File opening failed */
  if (fp == (FILE*)NULL)
      /* Return an error */
      return NULL;

  /* File opened successfully */
  else
    {
      /* Buffer for reading the file */
      char s[BUFFER_MAX_SIZE];
      /* Pointers (start/end) used for parsing integers */
      char* s_ptr = (char*)NULL;
      char* e_ptr = (char*)NULL;
      /* While we can read a line from file */
      while ((s_ptr = fgets(s, BUFFER_MAX_SIZE-1, fp)) != (char*)NULL)
        {
          do
            {
              /* Parse integer from start and record the end */
              int n = (int)strtol(s_ptr, &e_ptr, 10);
              /* If end is a comma ',' */
              if (*e_ptr == ',')
                /* For next integer, starts after comma */
                s_ptr = e_ptr+1;
              else
                /* Else start where we stopped */
                s_ptr = e_ptr;
              /* Append read integer as a new list element */
              list = append_list(list, n);
            }
          /* While we didn't reach end of line or end of file */
          while(*s_ptr != '\n' && *s_ptr != EOF);
        }
    }
  /* Return the new list */
  return list;
}

/*
 * save_list
 *
 * Save a linked list to the file named <filename>.
 * If saving fails, NULL is returned.
 * params:
 *   filename - in: the name of the file to save the list to
 * returns:
 *   the saved list if successfull
 *   NULL otherwise
 */
listechainee_ptr
save_list(listechainee_ptr list, char* filename)
{
  /* Opens file 'filename' for writing */
  FILE *fp = fopen(filename, "w");
  /* If file didn't opened successfully */
  if (fp == (FILE *)NULL)
      /* Returns error */
      return NULL;

  /* File opened successfully */
  else
    {
      /* Browse the list */
      listechainee_ptr tmp = list;
      while (tmp)
        {
          /* Print the current element */
          fprintf(fp, "%d", tmp->N);
          /* If there is still some elements, add a comma */
          if (tmp->next)
            fprintf(fp, ",");
          /* Next element */
          tmp = tmp->next;
        }
      /* At end of list add a new line */
      fprintf(fp, "\n");
      /* Close the file */
      fclose(fp);
    }
  /* Return the list */
  return list;
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
  /* Take head of list */
  listechainee_ptr tmp = list;
  /* Browse the list */
  while(tmp)
    {
      /* If element was found */
      if (tmp->N == n)
        /* Returns success */
        return TRUE;
      /* Next element */
      tmp = tmp->next;
    }
  /* Returns failure */
  return FALSE;
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
    return FALSE;
  /* Local index */
  int lix = -1;
  /* Temp node ptr */
  listechainee_ptr tmp = list;
  /* Browse the list */
  while(tmp)
    {
      /* Incr local index (next one) */
      lix++;

      /* If index is found */
      if (lix == ix)
        /* Return Yes */
        return TRUE;
      /* Goes on next node */
      tmp = tmp->next;
    }

  /* Not found */
  return FALSE;
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
  /* Test that element not already in the list */
  if (test_elem_in_list(list, n))
    /* If yes, returns NULL */
    return NULL;
  
  /* Allocate new node */
  listechainee_ptr new_node = (listechainee_ptr)malloc(sizeof(struct listechainee_st));
  /* Init new node value */
  new_node->N = n;
  new_node->next = (listechainee_ptr)NULL;
  /* If list is empty */
  if (list == NULL)
    /* Set the new node as the new list */
    list = new_node;

  else
    {
      /* Get a ptr to current node (at head) */
      listechainee_ptr tmp = list;
      /* While not at end of list */
      while(tmp)
        {
          /* If at tail of list */
          if (tmp->next == NULL)
            /* Exits loop */
            break;
          /* Advance to the next node */
          tmp = tmp->next;
        }
      /* Link the new node as tail of list */
      tmp->next = new_node;
    }

  /* Returns list */
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
  /* Test that element not already in the list */
  if (test_elem_in_list(list, n))
    /* If yes, returns NULL */
    return NULL;

  /* Allocate new node */
  listechainee_ptr new_node = (listechainee_ptr)malloc(sizeof(struct listechainee_st));
  /* Init new node value */
  new_node->N = n;
  /* Linked the list as tail of new node */
  new_node->next = list;

  /* Returns the new node */
  return new_node;
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
                  list->next = newn;
                }
              /* Insert in the list (after the first element) */
              else
                {
                  /* Link new node to next node */
                  newn->next = tmp->next;
                  /* Insert new node */
                  tmp->next = newn;
                }
              break;
            }
          /* Goes on to next node */
          tmp = tmp->next;
          /* And incr local ix */
          lix++;
        }      
    }

  return list;
}

int
find_elem_ix(listechainee_ptr list, int n)
{
  int ix = -1;

  /* Take head of list */
  listechainee_ptr tmp = list;
  /* Browse the list */
  while(tmp)
    {
      ix++;
      /* If element was found */
      if (tmp->N == n)
        /* Returns success */
        return ix;
      /* Next element */
      tmp = tmp->next;
    }
  
  return -1;
}

/*
 * delete_end_of_list
 *
 * Delete tail of the list
 * params:
 *  list - in: the list in which to remove tail
 * returns:
 *  The list or NULL if list empty
 */
listechainee_ptr
delete_end_of_list(listechainee_ptr list)
{
    listechainee_ptr tmp = list;

  /* If list not empty and has more than 1 element */
  if (tmp && tmp->next)
    {
      /* Find the end of list */
      while (tmp && tmp->next)
        {
          /* If end of list found */
          if (tmp->next && tmp->next->next == (listechainee_ptr)NULL)
            {
              /* Free it and exit loop */
              free((void*)tmp->next);
              tmp->next = (listechainee_ptr)NULL;
              break;
            }

          /* Move to the next node */
          tmp = tmp->next;
        }
    }
  /* If list is not empty but has only 1 element */
  else if (list && !list->next)
    {
      /* Free it */
      free(list);
      /* and return an empty list */
      list = (listechainee_ptr)NULL;
    }

  return list;
}

/*
 * delete_start_of_list
 *
 * Delete head of the list
 * params:
 *  list - in: list in which to remove head from
 * returns:
 *  The list or NULL if list empty
 */
listechainee_ptr
delete_start_of_list(listechainee_ptr list)
{
  listechainee_ptr tmp = (listechainee_ptr)NULL;
  if (list)
    {      
      tmp = list->next;
      free((void*)list);
    }
  return tmp;
} 

/*
 * delete_elem_in_list
 *
 * Delete an element from the list provided as parameter
 * params:
 *  list - in: the list in which to remove an element from
 *  n    - in: the element to remove
 * returns:
 *  list or NULL if list empty
 */
listechainee_ptr
delete_elem_in_list(listechainee_ptr list, int n)
{
  /* Test that element is in list */
  if (test_elem_in_list(list, n))
  {
    /* If first element to remove */
    if (list->N == n)
      {
        /* keep ptr to head for freeing */
        listechainee_ptr head = list;
        /* Change head of list */
        list = list->next;
        /* Free node */
        free((void*)head);
      }
    else
      {
        /* Keep ptr to the previous node */
        listechainee_ptr prev = (listechainee_ptr)NULL;
        /* Get a ptr to the current node */
        listechainee_ptr ptr = list;
        /* Search the node to remove: while not found ... */
        while(ptr && ptr->N != n)
          {
            /* Move previous node */
            prev = ptr;
            /* Get next node */
            ptr = ptr->next;
          }
        /* If the node to remove was found */
        if (ptr->N == n)
          {
            /* If the previous node is available */
            if (prev != NULL)
              /* Unlink the node to remove by linking prev to the found node next ptr */
              prev->next = ptr->next;
            else
              /* Set the head of the list as the found node next ptr */
              list = ptr->next;

            /* Free the found node */
            free((void*)ptr);
          }
      }
  }

  /* Return the list (if head has been removed) */
  return list;
}


/**
 * vim: et:ts=4:sw=4:sts=4																																				   
 * -*- mode: C; coding: utf-8-unix; tab-width: 4; tab-always-indent: t; tab-first-completion: nil -*-
 */
