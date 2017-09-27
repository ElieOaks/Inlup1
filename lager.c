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

typedef void(goods_func)(goods_t *elem);

#define QUIT 'Q'

#define LIST 'L'
#define LEFT 'L'

#define UNDO 'U'
#define PRODUCT 'U'

#define REMOVE 'R'
#define RIGHT 'R'

#define ADD 'A'

#define EDIT 'E'
#define EXIT 'E'



//------------------------------- All functions in this file: -----------------------
void add_goods(tree_t *tree);
shelf_t *make_shelf(int amount, char* hylla);
char *ask_question_key(char *question, tree_t *tree);
goods_t *new_goods(char *name, char *desc, int price, list_t *shelves);
void list_goods(tree_t *tree, goods_func);   
void list_shelves (goods_t *goods);
void show_goods(tree_t *tree, K name);
void index_action(tree_t *tree, int page, goods_func function);
                    


// ------------------------------list good ---------------------------------------
void list_shelves (goods_t *goods)
{
  list_t *shelves = goods->shelves;
  int length = list_length(shelves);
  for (int i = 0; i != length; i++)
  {
    shelf_t *cur_shelf = (shelf_t *) list_get(shelves, i);
    printf("Shelf: %s  Amount: %d  \n", cur_shelf->Hylla, cur_shelf->Antal);
  }
  return;
}

void show_goods(tree_t *tree, K name)
{
  goods_t *goodet = (goods_t *) tree_get(tree, name);

  
  
  // TODO: gör om picked_goods till en goods_t
  printf("Name: %s \n", goodet->Name);
  printf("Description: %s \n", goodet->Desc);
  printf("Price: %d \n", goodet->Price);

  list_shelves(goodet);
  
}
//-------------------------------Add goods ----------------------------

list_t *make_shelves_list(int shelf_amount)
{
  list_t *new_l = list_new();
  while (shelf_amount != 0)
    {
      char *hylla =  ask_question_hylla("Place goods on what shelf?\n");
      int amount = ask_question_int("Amount of goods on this shelf?\n");
      shelf_t *new_shelf = make_shelf(amount, hylla);
      list_prepend(new_l, new_shelf);
      --shelf_amount;
    }
  return new_l;
}


void add_goods(tree_t *tree)
{
  K name = ask_question_key("Name goods\n", tree); //två objekt får ej ha samma namn. 
  char *desc = ask_question_string("Describe goods\n");
  int price = ask_question_int("Price of goods\n");
  int hyllor = ask_question_int("place on how many shelves?");
  list_t *shelves_list = make_shelves_list(hyllor);

  goods_t *new_good = new_goods(name, desc, price, shelves_list);
  tree_insert(tree, name, new_good);
  return;
}


shelf_t *make_shelf(int amount, char* hylla) //Måste frigöras!
{
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  new_shelf->Hylla = hylla;
  new_shelf->Antal = amount;
  return new_shelf;
}

goods_t *new_goods(K name, char *desc, int price, list_t *shelves)
{
  goods_t *new_g = calloc(1, sizeof(goods_t));
  new_g->Name = name;
  new_g->Desc = desc;
  new_g->Price = price;
  new_g->shelves = shelves; // TODO - kan inte placera varor på tagna hyllor. 

  return new_g;
}

//----------------------------------------------------


char *ask_question_key(char *question, tree_t *tree)
{
  K answer;
  answer = ask_question_string(question);

  printf("Answer is: %s", answer);
  bool already_key = tree_has_key(tree, answer);

  printf (already_key ? "\nTrue\n" : "\nFalse\n");
  
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

void does_nothing(goods_t *elem)
{
  return;
}

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

K index_to_key(tree_t *tree, int index, int page)//tar in korrekt index, inte vad väljaren skriver!!
{
  K *buf = tree_keys(tree);
  int goods_amount = tree_size(tree);
  index = index + 20 * page;
  if (index < goods_amount)
    {
      K choice = buf[index]; //-1 as list is indexed from 1, not 0.
      return choice;
    }
  else
    {
      puts("Your number is out of bounds, please choose a nr from available goods");
      return NULL;
    }
}

void index_action(tree_t *tree, int page, goods_func function)
{
  int num = ask_question_int("Choose product nr:\n");
  K key_name = index_to_key(tree, (num-1), page);
  show_goods(tree, key_name);
  goods_t *elem = (goods_t *) tree_get(tree, key_name);
  function(elem);  
}


void list_goods(tree_t *tree, goods_func function)
{
  int tot_pages = tree_size(tree) / 20 + (tree_size(tree)%20 > 0 ? 1 : 0); // amount of pages as needed + 1 page if amount is not devisable by 20. 
  int first_page = 0;
  int *cur_page = &first_page;
   list_helper(tree, *cur_page);

  while(true)
    {
      printf("\nTotalt antal varor: %d\n", tree_size(tree));
      char choice = ask_question_menu("\n\n[L]eft <-\t"
                                      "Choose prod[u]t"
                                      "-> [R]ight\t"
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

        case PRODUCT:
          index_action(tree, *cur_page, function);
          break;

          
	case EXIT:
          return;          
        }
    }
  return;
}
//----------------------------------undo action ---------------------------------------

void undo_action();

/*
goods_t copy_elem(goods_t *elem)
{
  char *name = elem->Name;
  char *desc = elem->Desc;
  int pris = elem->Price;
  list_t *shelf = elem->shelves;
  
  
}


tree_t *copy_tree(tree_t *tree)
{
  tree_t *copy = tree_new();
  int tree_siz = tree_size(tree);
  K *keys = tree_keys(tree);
  for (int i = 0; i <= tree_siz; i++)
    {
      goods_t *elem = (goods_t *) tree_get(tree, keys[i]);
      goods_t elem_copy = copy_elem(elem);
      tree_insert(copy, *keys, elem);
    }
  return copy;
}
*/
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
	  list_goods(tree, does_nothing);
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
