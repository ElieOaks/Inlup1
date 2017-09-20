#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct link link_t;

struct list {
  link_t *first;
  link_t *last;
};

struct link {
  int value;
  link_t *next;
};

list_t *new_list()
{
  list_t *new = calloc(1, sizeof(list_t));
  return new;
}

link_t *new_link(L elem)
{
  link_t *new = calloc(1, sizeof(link_t));
  new->value = elem;
  return new;
}

void list_append(list_t *list, L elem)
{
  link_t *old = (list->last);
  link_t *new = new_link(elem);
  if (old)
    {
      old->next = new;
      list->last = new;
    }
  else
    {
      list->last = new;// first eller last?
      list->first = new;
      
    }
}


void list_prepend(list_t *list, L elem)
{
  // new är den nya länken som skapas av elem
  // new ska petas in så att den ligger FÖRE den nuvarande first
  link_t *new = new_link(elem);
  new -> next = list -> first;
  list -> first = new;
    
}


void list_print(list_t *list)
{
  if (list->first == NULL)
    {
      printf("FEL \n");
    }
  else
    {
      link_t *current = list->first;
      while(current != NULL)
        {
          int num = current->value;
          printf ("%d \n", num);
          current = current->next;
        }
    }
}

bool list_insert(list_t *list, int index, L elem)
{
  int l_length = list_length(list);
  link_t *n_link = new_link(elem);
  if ((list->first == NULL && index != 0) || (index > l_length || index < -(l_length+1))) // lägg till -1 
    {
      printf("Invalid input!\n");
      return false;
    }
  else if (index == 0 || index == -(l_length+1)) // lägg till -1
    {
      list_prepend(list, elem);
      return true;
    }
  else if ( index == l_length || index == -1 )
    {
      list_append(list, elem);
      return true;
    }
  else
    {
      link_t *current = list->first;
      for (int i = 0; i < index-1 || ( index < 0 && i < l_length+index); i++) // TODO: gör om detta till en funktion
        {
          current = current->next;
        }
      n_link->next = current->next;
      current->next = n_link;
      return true;
    }
  return false;
}

bool list_remove(list_t *list, int index, L *elem)
{
 int l_length = list_length(list);
  if ((list->first == NULL && index != 0) || (index >= l_length || index < -(l_length+1))) // lägg till -1 
    {
      printf("Invalid input!\n");
      return false;
    }
  else if (index == 0 || index == -(l_length+1)) // lägg till -1
    {
      link_t *temp = list->first;
      list->first = list->first->next;
      free(temp);
      return true;
    }
  else
    {
      link_t *current = list->first;
      for (int i = 0; i < index-1 || ( index < 0 && i < l_length+index-1); i++)
        {
          current = current->next;
        }
      link_t *temp = current->next;
      current->next = current->next->next;
      free(temp);
      return true;
    } 
  return false;
}

void list_remove_all (list_t *list)
{
  L temp;
  int length = list_length(list);
  for (int i = length; i != 0; i--)
    {
      list_remove(list, 0, &temp);
    }
  free(list);
}

L *list_get(list_t *list, int index)
{
  int length = list_length(list);
  link_t *current = list->first;
  
  if (list->first == NULL || index < -length || index >= length)
    {
      printf("Index is out of bounds. Please try again. \n");
      return false;
    }
  else
    {
      for (int i = index; (i != 0 && i > -length); --i)
        {
          current = current->next;
        }
      L *cur_value = &(current->value);
      return cur_value;
      
    
    }
}

L *list_first(list_t *list)
{
  L *first_value = &(list->first->value);
  return first_value;
}

L *list_last(list_t *list)
{
   L *last_value = &(list->last->value);
  return last_value;
}


int list_length(list_t *list)
{
  link_t *current = list->first;
  int count = 0;
  for (; current != NULL; count++)
    {
      current = current->next;
    }
  return count;
}

void list_delete(list_t *list, list_action cleanup);

void list_apply(list_t *list, list_action2 fun, void *data);

int main (void)
{
  list_t *list = new_list();
  list_append(list, 3);
  list_append(list, 2);
  list_append(list, 1);
  list_prepend(list, 4);
  list_prepend(list, 5);
  //list_insert(list, 2, 10); // här läggs det in skit på ett valt index
  //L elem = 22; // 
  list_get(list, 0);
  list_print(list); // här skrivs allt ut
  list_remove_all(list);
}


