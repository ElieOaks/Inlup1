#include <stdbool.h>
#include "list.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct link link_t;

struct list
{
  link_t *first;
  link_t *last;
};

struct link
{
  L value;
  link_t *next;
};

list_t *new_list()
{
  list_t *new_l = calloc(1, sizeof(list_t));
  new_l->first = NULL;
  new_l->last = NULL; //Behövs detta dp calloc redan nollställer det?
  return new_l;
}

link_t *link_new(L elem)
{
  link_t *new = malloc(sizeof(link_t));
  new->value = elem;
  return new;
}

void list_append(list_t *list, L elem)
{
  link_t *old_last = list->last;
  link_t *new_last = link_new(elem);
  if (old_last != NULL) //om old last inte är null
    {
      old_last->next = new_last; // låt den gamla last peka på nya last
      list->last = new_last; //last pekar nu på våran nya last
    }
  else
    {
      list->last = new_last;
      list-> first = new_last; //om listan är tom, peka sista på nya elementet. 
    }
}


void list_prepend(list_t *list, L elem)
{
  link_t *new = link_new(elem);
  if (list->first != NULL)
    {
      new->next = list->first; //nya länken pekar på det som var först
      list->first = new;  // Nya länken blir first.
    }
  else
    {
      list->first = new;
      list->last = new;
    }
}

void print_list(list_t *list)
{
  if (list->first == NULL)
    {
    puts("Empty list");
    }
  else
    {
    link_t *current = list->first;
    while (current != NULL)
      {
        int nr = current->value;
        printf("%d \n", nr);
        current = current->next;
      }
    }
}


int list_length(list_t *list)
{
  link_t *current = list->first;
  int count = 0;
  for (;current != NULL; count++)
    {
      current = current->next;
    }
  return count;
}

bool list_insert(list_t *list, int index, L elem)
{
 link_t *n_link = link_new(elem); 
 int length_l = list_length(list);
 if (index == 0 || index == -(length_l+1))
   {
     list_prepend(list, elem);
     return true;
   }
 else if (index == length_l || index == -1)
    {
      list_append(list, elem);
      return true;
    }
 else if ( list->first == NULL || index < -length_l || index > length_l)
   {
     return false;
   }
 else
   {
     link_t *current = list->first;
     for (int c = 0; c < index - 1 || ( index < 0 && c < index + length_l ); c++)
       {
         current = current->next;
       }   
     n_link->next = current->next;
     current->next = n_link;
     return true;
   }
}

bool list_remove(list_t *list, int index, L *elem)
{
 int length_l = list_length(list);
 if ( list->first == NULL || index < -length_l || index >= length_l)
   {
     return false;
   }
 else if (index == 0 || index == -length_l) //remove first element
   {
     link_t *temp = list->first;
     *elem = list->first->value;               //sparar värdet från den borttagna länken i param. elem.
     list->first = list->first->next;
     free(temp);
     return true;
   }
 else
   {
     link_t *current = list->first;
     for (int c = 0; c < index-1 || ( index < 0 && c < index + length_l-1); c++)
       {
         current = current->next;
       }
     link_t *temp = current->next;
     *elem = current->value;                   //sparar värdet från den borttagna länken i param. elem.
     current->next = current->next->next;
     free(temp);
     return true;
   }
}

void remove_whole_list(list_t *list)
{
  L value = 0; //för att list_remove skall fungera, men då listan ska bort behöver värderna ej sparas. 
  int len = list_length(list);
  int counter = 1;
  for (; counter != len+1; ++counter)
    {
      list_remove(list, 0, &value); // ändra så den f linjär komplex. len = 0.
      printf("\ndid\n");
    }
  free(list);  
}

// --------------------------------------------REKURSIV BORTTAGNING FÖR LIST ------------------------------------

void rec_remove_links(link_t *cur)
{
  if (cur== NULL)
    {
      return;
    }
  else
    {
      rec_remove_links((cur->next)); // -------------------------------------demonstrera för tidig friigörning, om free(cur) var ovanför. 
      free(cur);
      puts("true"); //för att kolla att den fungerar.
    }
}

void rec_remove_whole_list(list_t *list)
{
  link_t *cur = list->first;
  rec_remove_links(cur);
  free(list); // glömde att frigöra detta, här hjälpe valgrind genom att peka ut calloc och new_list, alltså listan i sig inte hade frigjprts efter att den skapats. 
}


// --------------------------------------------REKURSIV BORTTAGNING FÖR LIST ------------------------------------

L *list_get(list_t *list, int index) // Återskapa tidgare fel. 
{
  int len = list_length(list); 
  link_t *current = list->first;

 if ( list->first == NULL || index < -len || index >= len)
   {
     puts("index out of bounds");
      return 0;
   }
 else
   {
   while ((index != 0 && index > -len))
     {
       current = current->next;
       --index;
     }
 L *val = &(current->value);
 return val;
   }
}

L *list_get2(list_t *list, int index) // först bara för positiva index - det återskapade problemet. 
{
  link_t *current = list->first;
  while (index != 0 && current != NULL)
    {
      current = current->next;
      --index;
    }
  L val = current->value;
  return val;
}

L *list_first(list_t *list);

L *list_last(list_t *list);


int main()
{
  list_t *list = new_list();
  list_prepend(list, 7);
  list_prepend(list, 6);
  list_prepend(list, 5);
  list_prepend(list, 4);
  list_prepend(list, 3);
  list_prepend(list, 2);
  list_prepend(list, 1);
  print_list(list);
  //list_insert(list, 0, 0);
  L num = list_get2(list, 0);
  printf("I got: %d\n", num);
  rec_remove_whole_list(list);
  return 0;
}



