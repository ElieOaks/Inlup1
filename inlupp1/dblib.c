/* dblib innehåller allt som fanns i:
  - utils.h
  - utils.c
  - db.c       */

extern char *strdup(const char *);

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dblib.h"

/*Valida kommandon för menyn*/
#define VALIDSTRING "LlTtRrGgHhAa"

answer_t ask_question(char *question, check_func check, convert_func convert)
{
  int buf_siz = 255;
  char buf[buf_siz];
  int count = 0; 
  
  do
  {
    if (count > 0)
      {
        printf("Invalid input '%s'. Please try again. \n", buf);
      }
    printf("%s\n", question);
    read_string(buf, buf_siz);
    count++;
    
  }
  while (!check(buf));
  return convert(buf);

}

/* Kontrollfunktion till ask_question_menu*/

bool is_valid_input(char *input)
{
  
  char *valid = VALIDSTRING; // är typen rätt?
s  if (strlen(input) != 1)
    {
      return false;
    }
  int length = strlen(valid);
  
  for (int i = 0; i < length; i++)
    {
      char cur = valid[i];
      if (cur == input[0])
	{
	  //printf("Returning true");
	  return true;
	}
    }
  
  printf("Invalid input, please choose again. \n");
  return false;
  
}

answer_t toupper2(char *str)
{
  return (answer_t) toupper(str[0]);
}

char ask_question_menu(char *question) // frågan här kommer bli menyalternativen (print_menu())
{
  answer_t answer = ask_question(question, is_valid_input, toupper2);
  return answer.c;
}

bool is_shelf (char *shelf)
{
  int length = strlen(shelf);
  if (isalpha(shelf[0]) && length == 3)
    {
    for (int i = 1;i < length; i++)
      {
        if (!isdigit(shelf[i]))
          {
            return false; 
          }
      }
    }
  else
    {
      return false;
    }

  return true;
}

char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func) strdup).s;
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func)atoi);
  return answer.i;
}

/*Skiten buggar - kan inte ha ett för långt namn på ett item
hoppar över description.*/
int read_string(char *buf, int buf_siz)
{
  int count = 0;
  int symbol;
  for (; count < buf_siz; count++)
    {
      symbol = getchar();

      if (symbol != '\n' && symbol != EOF) // så länge som symbolen inte är Retur, 
        {
          buf[count] = symbol;
        }
      else
        {
          buf[count] = '\0'; // nu är strängen slut
          return count;
        }
    }
  return count;  
}

bool not_empty(char *str)
{
  return strlen(str) > 0;
}


char *ask_question_string(char *question) // läsa en sträng, returnera den lästa strängen?
{
  return ask_question(question, not_empty, (convert_func) strdup).s;
}



bool is_number (char *str)
{
  int length = strlen(str);
  if (length == 1 && str[0] == '-')
    {
      return false;
    }
  if ((str[0] == '-') || isdigit(str[0]) )
    {
    for (int i = 1;i < length; i++)
      {
        if (!isdigit(str[i]))
          {
            return false; 
          }
      }
    }
  else
    {
      return false;
    }

  return true;
}



//////////* line. 166 - Här börjar alla db-funktioner!*//////////

void print_item(item_t *item)
{
  printf("Name: %s\n", item -> name);
  printf("Description: %s\n", item -> desc);
  printf("Price: %d\n", item -> price/100); //TODO: priset ska visas i ören
  printf("Shelf: %s\n", item -> shelf);
}

item_t make_item(char *name, char *desc, int price, char* shelf)
{
  item_t item = { .name = name, .desc = desc, .price = price, .shelf = shelf};
  return item;
}


item_t input_item(void)
{
  char *name = ask_question_string("Input the name of the item: \n");
  char *desc = ask_question_string("Input description of the item: \n");
  int price = ask_question_int("Input price: \n");
  char *shelf = ask_question_shelf("Input shelf: \n");

  item_t item = make_item(name, desc, price, shelf);
  return item; 
}

char *magick (char *array1[], char *array2[], char *array3[], int length)
{
  int random;
  char buf[255]; // den tomma arrayen som ska fyllas med kul skit!
  strcpy(buf, array1[random = rand() % length]);
  strcat(buf, "-");
  strcat(buf, array2[random = rand() % length]);
  strcat(buf, " ");
  strcat(buf, array3[random = rand() % length]);
  strcat(buf, "\0");
  return strdup(buf);
  
}

/*Menyfunktion- [L]ägg till en vara*/
void add_item_to_db(item_t *db, int *db_siz)
{
  item_t item = input_item();
  db[*db_siz] = item;
  *db_siz = *db_siz + 1; 
}

/*Menyfunktion - Lista [h]ela varukatalogen*/
void list_db (item_t *db, int db_siz)
{
  for (int i = 1; i <= db_siz; ++i)
   {
     printf("%d. Name: %s\n", i, db[i-1].name);
   }
}

/*Menyfunktion - [T]a bort en vara*/
void remove_item_from_db(item_t *db, int *db_siz)
{
  list_db(db, *db_siz);
  int to_remove = ask_question_int("Which item would you like to remove? Enter index.");
  if ( to_remove <= *db_siz && to_remove > 0)
    {
      printf("Here is the item saved at  index %d: \n", to_remove);
      print_item(&db[to_remove-1]);

      for (int i = to_remove; i < *db_siz; i++)
        {
          //TODO: skriv en loop ersätter det valda indexet med den som kommer efter
          db[to_remove-1] = db[to_remove];
        }
       // tills arrayen når sitt slut. Minska då arrayens storlek med 1.
      *db_siz = *db_siz - 1;
      printf("Removed! \n");
    }
}

/*Menyfunktion - [R]edigera en vara*/
void edit_db (item_t *db, int db_siz)
{
  list_db(db, db_siz);
  int to_edit = ask_question_int("Which item would you like to edit? Enter index.");
  if ( to_edit <= db_siz && to_edit > 0)
    {
      printf("Here is the item saved at  index %d: \n", to_edit);
      print_item(&db[to_edit-1]);
      db[to_edit-1] = input_item();
    }
  
  
}
