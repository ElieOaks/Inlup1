#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include "tree.h"
#include "dblib.h"


#define QUIT 'Q'
#define LIST 'L'
#define LEFT 'L'
#define EDIT_SHELF 'L'
#define UNDO 'U'
#define PRODUCT 'U'
#define EDIT 'E'
#define EXIT 'E'
#define EDIT_DESC 'E'
#define REMOVE 'R'
#define RIGHT 'R'
#define EDIT_PRICE 'R'
#define ADD 'A'
#define EDIT_AMOUNT 'A'


struct goods {
  char *Name;
  char *Desc;
  int Price;
  list_t *Shelves; // pekar på listan av ALLA hyllor som finns! 
};

typedef struct goods goods_t;
typedef void(goods_func)(goods_t *elem);

void do_nothing(goods_t *elem)
{
  return;
}

void show_goods(tree_t *tree, K name);
void list_shelves (goods_t *goods);
void edit_goods(goods_t *goods);

/*--- Add Goods---------------------------------------------------------------------------------*/
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

goods_t* new_goods(char *name, char *desc, int price, list_t *shelves)
{
  goods_t *new_g = calloc(1, sizeof(goods_t));
  new_g->Name = name;
  new_g->Desc = desc;
  new_g->Price = price;
  new_g->Shelves = shelves;
  
  return new_g;
  
}

list_t *make_shelves_list(int shelf_amount)
{
  list_t *new_l = list_new();
  while (shelf_amount != 0)
    {
      char *hylla =  ask_question_shelf("Place goods on what shelf:\n");
      int amount = ask_question_int("Input amount on this shelf:\n");
      shelf_t *new_shelf = make_shelf(amount, hylla);
      list_prepend(new_l, new_shelf);
      --shelf_amount;
    }
  return new_l;
}

void add_goods(tree_t *tree)
{
  char *name = ask_question_key("Name of the goods?: \n",tree);
  char *desc = ask_question_string("Describe the goods: \n");
  int price = ask_question_int("Give the goods a price: \n");
  int shelf = ask_question_int("Place on how many shelves?: \n");
  list_t *shelves_list = make_shelves_list(shelf);

  goods_t* new_good = new_goods(name, desc, price, shelves_list);
  printf("Name: %s \n", new_good->Name);
  printf("Description: %s \n", new_good->Desc);
  printf("Price: %d \n", new_good->Price);
  list_shelves(new_good);
  printf("Do you want to add this product in the catalouge? \n");
  char answer = ask_question_menu("\n Y[e]s \t"
                                     "N[a]h \t"
                                      "Ed[u]it\n");
  if (answer == 'E')
    {
      tree_insert(tree, name, new_good);
      printf("\nThe product was successfully added! \n");
    }
  else if (answer == 'A')
    {
     printf("\n Canceled. The product was not added. \n");
     return;
    }
  else if (answer == 'U')
    {
      edit_goods(new_good);
    }

  
}

// add goods - end------------------------------------------------------------------//

void remove_goods()
{
  return;
}


/*--- List goods ----------------------------------------------------------------------*/

void list_shelves (goods_t *goods)
{
  list_t *shelves = goods->Shelves;
  int length = list_length(shelves);
  for (int i = 0; i != length; i++)
  {
    shelf_t *cur_shelf = (shelf_t *) list_get(shelves, i);
    printf("%d. Shelf: %s  Amount: %d  \n",(i+1), cur_shelf->shelf, cur_shelf->amount);
  }
  return;
}

// TODO: den seggar här
void show_goods(tree_t *tree, K name)
{
  goods_t *picked_goods = (goods_t *) tree_get(tree, name);
  
  // TODO: gör om picked_goods till en goods_t
  printf("Name: %s \n", picked_goods->Name);
  printf("Description: %s \n", picked_goods->Desc);
  printf("Price: %d \n", picked_goods->Price);

  // peka på shelves->shelf och ->amount
  list_shelves(picked_goods);
 
  
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

// funktionen som list_func syftar på
K index_to_key(tree_t *tree, int index, int page)//tar in korrekt index, inte vad väljaren skriver!!
{
  K *buf = tree_keys(tree);
  int goods_amount = tree_size(tree);
  index = index + 20 * page;
  if (index <= goods_amount)
    {
      K choice = buf[index]; //-1 as list is indexed from 1, not 0.
      return choice;
    }
  else
    {
      puts("Your number is out of bounds, please choose an index from available goods");
      return NULL;
    }
}

void choose_product(tree_t *tree, int page, goods_func function)
{
 int index = ask_question_int("Input the index of the product: \n");
 if (tree_size(tree) == 0)
   {
     printf("\nThe catalouge is empty! Please add a product and try again. \n");
     return;
   }
 K chosen_product = index_to_key(tree, index-1, page );
 show_goods(tree, chosen_product);
 goods_t *product = (goods_t *) tree_get(tree, chosen_product);
 function(product);
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
                                      "Choose prod[u]ct \t"
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

        case PRODUCT:
          choose_product(tree, *cur_page, function);
          printf("\n");
          list_helper(tree, *cur_page);
          break;

          
	case EXIT:
          return;          
        }
    }
  return;
}


/*--- Edit goods --------------------------------------------------------------------------*/

void edit_shelves(goods_t *goods)
{
  list_shelves(goods);
  int index = ask_question_int("Input index of the shelf you want to change: \n");
  shelf_t *picked_shelf =(shelf_t *)list_get(goods->Shelves, index-1);
  printf("\nCurrent shelf: %p \n", picked_shelf->shelf);
  printf("----------------------------------------------------\n");
  char *new_shelf = ask_question_shelf("New shelf (note that the amount will be the same): ");

  picked_shelf->shelf = new_shelf;

  
}
 
void edit_goods(goods_t *goods)
{
  while (true)
    {
   char answer = ask_question_menu("\nD[e]scription \n"
                                      "P[r]ice \n"
                                      "She[l]f \n"
                                      "[A]mount \n"
                                      "Choose a row to edit or [q]uit: \n");

   switch (answer)
       {
        case EDIT_DESC:
          printf("\nCurrent description: %s \n", goods->Desc);
          printf("----------------------------------------------------\n");
          char *new_desc = ask_question_string ("New description: ");
          goods->Desc = new_desc;
         break;
        case EDIT_PRICE:
          printf("\nCurrent price: %d \n", goods->Price);
          printf("----------------------------------------------------\n");
          int new_price = ask_question_int("New price: ");
          goods->Price = new_price;
         break;
       case EDIT_SHELF:
         edit_shelves(goods);
         break;
       case EDIT_AMOUNT:
         printf("Not yet implemented! \n");
         break;
       case QUIT:
         printf("\n Your edits have been saved! \n");
         return;
         break;
         
       }
    }
  

}
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
         list_goods(tree, edit_goods);
         break;
       case UNDO:
         printf("Not yet implemented! \n");
         break;
       case LIST:
         list_goods(tree, do_nothing);
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
  event_loop(tree);
  }


