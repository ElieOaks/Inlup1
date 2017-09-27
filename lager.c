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

struct action
{
  int type; // NOTHING = 0, ADD = 1, REMOVE = 2, EDIT = 3
  goods_t *orig;
  goods_t *copy;
};

typedef struct action action_t;
typedef void(goods_func)(goods_t *elem, action_t *undo);

#define QUIT 'Q'

#define LIST 'L'
#define LEFT 'L'
#define SHELVES 'L'

#define UNDO 'U'
#define PRODUCT 'U'

#define REMOVE 'R'
#define RIGHT 'R'
#define PRICE 'R'

#define ADD 'A'
#define AMOUNT 'A'

#define EDIT 'E'
#define EXIT 'E'
#define DESCRIPTION 'E'



//------------------------------- All functions in this file: -----------------------
void add_goods(tree_t *tree);
shelf_t *make_shelf(int amount, char* hylla);
char *ask_question_key(char *question, tree_t *tree);
goods_t *new_goods(char *name, char *desc, int price, list_t *shelves);
void list_goods(tree_t *tree, goods_func, action_t *undo);   
void list_shelves (goods_t *goods);
void show_goods(tree_t *tree, K name);
void index_action(tree_t *tree, int page, goods_func function, action_t *undo);
goods_t *copy_goods(goods_t *goods);


// ------------------------------list good ---------------------------------------
void list_shelves (goods_t *goods)
{
  list_t *shelves = goods->shelves;
  int length = list_length(shelves);
  for (int i = 0; i != length; i++)
  {
    shelf_t *cur_shelf = (shelf_t *) list_get(shelves, i);
    printf("%d. Shelf: %s  Amount: %d  \n",(i+1), cur_shelf->Hylla, cur_shelf->Antal);
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




//------------------------------------------------------------------------------------
void remove_goods(tree_t *tree);

//--------------------------------- edit goods ---------------------------------------
void edit_shelves(goods_t *vara)
{
  int index = ask_question_int("Choose index of shelf name to edit!\n");
  shelf_t *shelf = (shelf_t *) list_get(vara->shelves, (index-1));
  
  char *hylla = ask_question_hylla("What is the new shelf?\n");

  shelf->Hylla = hylla; 
}

void edit_shelves_amount(goods_t *vara)
{
  int index = ask_question_int("Choose index of shelf amount to edit!\n");
  shelf_t *shelf = (shelf_t *) list_get(vara->shelves, (index-1));
  
  int amount = ask_question_int("What is the new amount on shelf?\n");

  shelf->Antal = amount;

}

void edit_goods(goods_t *vara, action_t *undo)
{
  undo->type = 3;
  undo->copy = copy_goods(vara);
  undo->orig = vara;
  while (true)
    {
      char choice = ask_question_menu("\nWhat do you want to edit?\n"
                                      "D[E]scription\n"
                                      "P[R]ice\n"
                                      "She[L}ves\n"
                                      "[A]mount\n"
                                      "[Q]uit from editor\n");
      switch(choice)
	{
	case DESCRIPTION:
          printf("Current description: %s\n", vara->Desc);
          puts("\n-----------------------------------------\n");
          char *new_desc = ask_question_string("New description:\n");
          vara->Desc = new_desc;            
	  break;
	case PRICE:
          printf("Curren price: %d\n", vara->Price);
          puts("\n-----------------------------------------\n");
          int new_price = ask_question_int("New price:\n");
          vara->Price = new_price; 
          break;
	case SHELVES:
          list_shelves(vara);
          edit_shelves(vara);
          break;
        case AMOUNT:
          list_shelves(vara);
          edit_shelves_amount(vara);
	case QUIT:
          return;
        }
    }
  return;
}

// ----------------------------------list goods ---------------------------------------

void does_nothing(goods_t *elem, action_t *undo)
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
      printf("\n%d.%s\n",(i%20)+1, current);
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

void index_action(tree_t *tree, int page, goods_func function, action_t *undo)
{
  int num = ask_question_int("Choose product nr:\n");
  if (tree_size(tree) == 0)
    {
      printf("\nThe catalogue is Empty, please add a product and try again\n");
      return;
    }
  K key_name = index_to_key(tree, (num-1), page);
  show_goods(tree, key_name);
  goods_t *elem = (goods_t *) tree_get(tree, key_name);
  function(elem, undo);  
}


void list_goods(tree_t *tree, goods_func function, action_t *undo)
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
          index_action(tree, *cur_page, function, undo);
          list_helper(tree, *cur_page);
          break;

          
	case EXIT:
          return;          
        }
    }
  return;
}
//----------------------------------undo action ---------------------------------------

void undo_action(tree_t *tree, action_t *action)
{
  if (action->type == 3)
    {
      *action->orig = *action->copy;
      puts("\nYour editing is undone!\n");
    }
  else
    {
      puts("There is nothing to undo\n");
    }
  action->type = 0;
}




goods_t *copy_goods(goods_t *goods)
{
  char *name = goods->Name;
  char *desc = goods->Desc;
  int pris = goods->Price;
  list_t *shelf = goods->shelves;

  goods_t *copy_g = new_goods(name, desc, pris, shelf);
  return copy_g;
}


//-------------------------------------------------------------------------------------

void exit_program()
{
  puts("Thank you, and goodbye!");
}

//------------------------------------------------------------------------------------
  
int event_loop(tree_t *tree)
{
  action_t undo = (action_t) {.type = 0}; 
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
          list_goods(tree, edit_goods, &undo);
          break;
	case UNDO:
          undo_action(tree, &undo);
          break;
	case LIST:
	  list_goods(tree, does_nothing, &undo);
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
