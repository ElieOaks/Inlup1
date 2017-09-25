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
  list_t *shelves; //pekar inte på shelf, utan på listan som innehåller alla shelves.
};

struct have_tree
{
  tree_t *old; //Trädet innan ändring
  tree_t *new; // Trädet efter ändring. Undo gör att *new = *old. 
};


#define QUIT 'Q'

#define LIST 'L'
#define LEFT 'L'

#define UNDO 'U'

#define REMOVE 'R'
#define RIGHT 'R'

#define ADD 'A'
#define EDIT 'E'
#define EXIT 'E'

//------------------------------- All functions in this file: -----------------------
void add_goods(tree_t *tree);
shelf_t *make_shelf(int amount, char* hylla);
char *ask_question_key(char *question, tree_t *tree);
goods_t new_goods(char *name, char *desc, int price, int amount, char *hylla);
void list_goods(tree_t *tree);
//-------------------------------Add goods ----------------------------

void add_goods(tree_t *tree)
{
  K name = ask_question_key("Name goods\n", tree); //två objekt får ej ha samma namn. 
  char *desc = ask_question_string("Describe goods\n");
  int price = ask_question_int("Price of goods\n");
  char amount = ask_question_int("Amount of goods?\n");
  char *hylla =  ask_question_hylla("Place goods on what shelf?\n"); //Ska inet kunna placera olika grejer på en hylla.

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

goods_t new_goods(K name, char *desc, int price, int amount, char *hylla)
{
  goods_t *new_g = calloc(1, sizeof(goods_t));
  new_g->Name = name;
  new_g->Desc = desc;
  new_g->Price = price;

  shelf_t *new_shelf = make_shelf(amount, hylla);

  list_t *new_shelf_list = list_new();
  list_append(new_shelf_list, new_shelf);

  new_g->shelves = new_shelf_list;

  return *new_g;
}

//----------------------------------------------------


char *ask_question_key(char *question, tree_t *tree)
{
  K answer;
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
void remove_goods(tree_t *tree);

void edit_goods();

// ----------------------------------list goods ---------------------------------------
void list_helper(tree_t *tree, int page) //20 per page. page index from 0. //
{
  K *buf = tree_keys(tree);
  int goods_amount = tree_size(tree);
  int i = 0 + 20 * page;

  for (; i !=  goods_amount && i < (20 + 20 * page); i++)
    {
      K current = buf[i];
      printf("%d.%s\n",(i%20)+1, current);
    }
  return;
}

void list_goods(tree_t *tree) //Printar ut två gånger när man trycker left och right. 
{
  int tot_pages = tree_size(tree) / 20 + (tree_size(tree)%20 > 0 ? 1 : 0); // amount of pages as needed.
  int first_page = 0;
  int *cur_page = &first_page;
   list_helper(tree, *cur_page);

  while(true)
    {
      printf("\nTotalt antal varor: %d\n", tree_size(tree));
      char choice = ask_question_menu("\n\n[L]eft <-\t"
                                      "[R]ight ->\t"
                                      "[E]xit\n");
      switch(choice)
	{

          
	case RIGHT:
          (*cur_page)++;
	  if (*cur_page < tot_pages)
            {
              list_helper(tree, *cur_page);
            }
          else
            {
              (*cur_page)--;
              puts("You have come to the end of the list");
              list_helper(tree, *cur_page);
            }
          break;

          
	case LEFT:
          (*cur_page)--;
	  if (*cur_page >= 0)
            {
              list_helper(tree, *cur_page);
            }
          else
            {
              (*cur_page)++;
              puts("You are at the beginning of the list");
              list_helper(tree, *cur_page);
            }
          break;

          
	case EXIT:
          return;          
        }
    }
  return;
}
  // -------------------------------------------------------------------------------------

void display_goods();

//----------------------------------undo action ---------------------------------------

void undo_action();

tree_t *copy_tree(tree_t *tree)
{
  tree_t *copy = calloc(1, sizeof(tree));
  *copy = *tree;
  return copy;
}

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
