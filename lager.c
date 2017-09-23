#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "list.h"
#include "dblib.h"

 
struct goods
{
  char *Name;
  char *Desc;
  int Price;
  list_t *shelf; //pekar inte på shelf, utan på listan som innehåller alla shelves.
};


#define QUIT 'Q'
#define LIST 'L'
#define UNDO 'U'
#define REMOVE 'R'
#define ADD 'A'
#define EDIT 'E'

//------------------------------- All functions in this file: -----------------------
char *ask_question_key(char *question, tree_t *tree);
goods_t new_goods(char *name, char *desc, int price, int amount, char *hylla);

//-------------------------------Add goods ----------------------------

void add_goods(tree_t *tree)
{
  char *name = ask_question_key("Name goods\n", tree);
  char *desc = ask_question_string("Describe goods\n");
  int price = ask_question_int("Price of goods\n");
  char amount = ask_question_int("Amount of goods?\n");
  char *hylla =  ask_question_hylla("Place goods on what shelf?\n"); //check new is_hylla function

  goods_t new_good = new_goods(name, desc, price, amount, hylla);
  tree_insert(tree, name, &new_good);
  return;
}

shelf_t *make_shelf(int amount, char* hylla) //Måste frigöras!
{
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  new_shelf->Hylla = hylla;
  new_shelf->Antal = amount;
  return new_shelf;
}

goods_t new_goods(char *name, char *desc, int price, int amount, char *hylla)
{
  goods_t *new_g = calloc(1, sizeof(goods_t));
  new_g->Name = name;
  new_g->Desc = desc;
  new_g->Price = price;

  shelf_t *new_shelf = make_shelf(amount, hylla);

  list_t *new_shelf_list = list_new();
  list_append(new_shelf_list, new_shelf);

  new_g->shelf = new_shelf_list;

  return *new_g;
}

//----------------------------------------------------


char *ask_question_key(char *question, tree_t *tree)
{
  char *answer;
  answer = ask_question_string(question);
  
  bool already_key = tree_has_key(tree, answer);
  if (already_key == false)
    {
      return answer;
    }
  else
    {
      printf("\'%s\' already exists. Choose a different name!\n", answer);
      return ask_question_key(question, tree);
    }
}




//--------------------------------------------------------------------
void remove_goods();
void edit_goods();

// ----------------------------------list goods ---------------------------------------
void list_goods(tree_t *tree)
{
  K *buf = tree_keys(tree);
  int len = tree_size(tree);
  for (int i = 0; i !=  len; i++)
    {
      K current = buf[i];
      printf("%d.%s\n",i+1, current);
    }
  free(buf);
  return;
}
// -------------------------------------------------------------------------------------

void display_goods();

//----------------------------------undo action ---------------------------------------

void undo_action();

//-------------------------------------------------------------------------------------

void exit_program()
{
  puts("Thank you, and goodbye!");
}

//------------------------------------------------------------------------------------
  
int event_loop(tree_t *tree)
{
  while (true)
    {
      char choice = ask_question_menu("\n[A]dd Item to database\n"
			     "[R]emove item from database\n"
			     "[E]dit database\n"
			     "[U]ndo last change\n"
			     "[L]ist database\n"
			     "[Q]uit\n\n"
			     "What would you like to do today?\n");
      switch(choice)
	{
	case ADD:
	  add_goods(tree);
	  break;
	case REMOVE:
          //remove_goods();
          break;
	case EDIT:
          //edit_goods();
          break;
	case UNDO:
          //undo_action;
          break;
	case LIST:
	  list_goods(tree);
	  break;
	case QUIT:
	  //puts("Bye bye");
	  return 0;
	  break;
	  
	}
    }


  return 0;
}

int main()
{
  tree_t *tree = tree_new();
  event_loop(tree);
  return 0;
}
