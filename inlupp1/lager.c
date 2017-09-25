#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include "tree.h"
#include "dblib.h"

#define QUIT 'Q'
#define LIST 'L'
#define UNDO 'U'
#define EDIT 'E'
#define REMOVE 'R'
#define ADD 'A'


struct goods {
  char *Name;
  char *Desc;
  int Price;
  list_t *Shelves; // pekar på listan av ALLA hyllor som finns! 
};

typedef struct goods goods_t;

/*--- Add Goods---*/
//ask_question för Name. precis som string fast kollar med tree_has_key om namnet redan finns


char *ask_question_key(char *question, tree_t *tree)
{
  char *answer = ask_question_string(question);

  if(tree_has_key(tree, answer))
    {
      printf("'%s' already exists. Please choose a different name. \n" , answer);
      return ask_question_key(question, tree);
    }
  else
    {
      return answer;
    }
}

shelf_t *make_shelf(int amount, char *shelf)
{
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  new_shelf->shelf = shelf;
  new_shelf->amount = amount;
  return new_shelf;
}

goods_t new_goods(char *name, char *desc, int price, char *shelf, int amount)
{
  goods_t *new_g = calloc(1, sizeof(goods_t));
  new_g->Name = name;
  new_g->Desc = desc;
  new_g->Price = price;

  // om hyllan inte redan finns! 
  shelf_t *new_shelf = make_shelf(amount, shelf);

  list_t *new_shelf_list = list_new();
  list_append(new_shelf_list, new_shelf);

  new_g->Shelves = new_shelf_list;
  
  return *new_g;
  
}

void add_goods(tree_t *tree)
{
  char *name = ask_question_key("Name of the goods?: \n",tree);
  char *desc = ask_question_string("Describe the goods: \n");
  int price = ask_question_int("Give the goods a price: \n");
  char *shelf = ask_question_shelf("Place it on what shelf?: \n");
  char amount = ask_question_int("Amount of goods? \n");

  goods_t new_good = new_goods(name, desc, price, shelf, amount);
  tree_insert(tree, name, &new_good);
  return;
  
}

// add goods - end---------------------------//

void remove_goods()
{
  return;
}
void edit_goods()
{
  return;
}

/*--- List goods ---*/

void list_shelves (goods_t *goods)
{
  list_t *shelves = goods->Shelves;
  int length = list_length(shelves);
  for (int i = 0; i != length; i++)
  {
    shelf_t *cur_shelf = (shelf_t *) list_get(shelves, i);
    printf("Shelf: %s  Amount: %d  \n", cur_shelf->shelf, cur_shelf->amount);
  }
  return;
}

void show_goods(tree_t *tree, K name)
{
  goods_t *picked_goods = (goods_t *) tree_get(tree, name);
  
  // TODO: gör om picked_goods till en goods_t
  printf("Name: %s \n", picked_goods->Name);
  printf("Description: %s \n", picked_goods->Desc);
  printf("Price: %int \n", picked_goods->Price);

  // peka på shelves->shelf och ->amount
  printf("Shelf:");
  list_shelves(picked_goods);
  printf("Amount: %d \n");
  
}

// TODO: Göra så att den visar sida för sida ala 20. for() i en for()!
void list_goods(tree_t *tree)
{
  K *buf = tree_keys(tree);
  int length = tree_size(tree);
  for (int i = 0; i != length; i++)
  {
    K current = buf[i];
    printf( "%d. %s \n", i+1, current);
    
  }
  free(buf);
  return;
}

// printar ut alla shelves


//------------ EVENT LOOP ----------//
int event_loop (tree_t *tree)
{
  while (true)
    {
      char answer = ask_question_menu("\n[A]dd goods \n"
                                      "[R]emove goods \n"
                                      "[E]dit goods \n"
                                      "[U]ndo latest change \n"
                                      "[L]ist the catalouge \n"
                                      "[Q]uit \n\n"
                                      "What would you like to do today? \n");

      switch (answer)
       {
        case ADD:
         add_goods(tree);
         break;
        case REMOVE:
         remove_goods();
         break;
       case EDIT:
         edit_goods();
         break;
       case UNDO:
         printf("Not yet implemented! \n");
         break;
       case LIST:
         list_goods(tree);
         break;
       case QUIT:
         printf("Cheers mate! \n");
         return 0;
         break;
         
       }
    }

  return 0;
}

int main (void)
{
  tree_t *tree = tree_new();
  /* tree_insert(tree, "Bajs", 44);
  tree_insert(tree, "Kratta", 33);
  tree_insert(tree, "Bajs", 77);
  tree_insert(tree, "Fitta", 99);
  tree_insert(tree, "Marsvin", 88);
  tree_insert(tree, "Lort", 11);
  tree_insert(tree, "Otto", 55);
  tree_insert(tree, "Anna", 22);
  tree_insert(tree, "Britta", 66);*/

  event_loop(tree);
  tree_delete(tree, NULL);
  }


