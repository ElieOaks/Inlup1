
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "dblib.h"
#include "tree.h"



// ----------------------Del 3 -------------------------

//------------------------ del 4----------------------------------

bool is_shelf(char *shelf)
{
  int nam_length = strlen(shelf);
  if (isalpha(shelf[0]))
    {
      for (int i = 1; i < nam_length; i++)
	{
	  if (!isdigit(shelf[i]))
	    {
	      return false;
	    }
	}
      return true;
    }
  return false;
}

char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func) strdup).s;
}

// ----------------------del 5 -------------------------------------



/*
int main(int argc, char *argv[])
{
  char *array1[] = {"kalas", "paj", "ägg", "bok"}; /// Lägg till!
  char *array2[] = {"ballonger", "glas", "fisk", "fyrverkerier" }; /// Lägg till!
  char *array3[] = {"dyrbar", "guld", "blåbär", "socker" }; /// Lägg till!

  if (argc < 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0;    // Antalet varor i arrayen just nu

    int items = atoi(argv[1]); // Antalet varor som skall skapas

    if (items > 0 && items <= 16)
    {
      for (int i = 0; i < items; ++i)
      {
        // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else
    {
      puts("Sorry, must have [1-16] items in database.");
      return 1; // Avslutar programmet!
    }

    for (int i = db_siz; i < 16; ++i)
      {
        char *name = magick(array1, array2, array3, 4); /// Lägg till storlek
        char *desc = magick(array1, array2, array3, 4); /// Lägg till storlek
        int price = random() % 200000;
        char shelf[] = { random() % ('Z'-'A') + 'A',
                         random() % 10 + '0',
                         random() % 10 + '0',
                         '\0' };
        item_t item = make_item(name, desc, price, shelf);

        db[db_siz] = item;
        ++db_siz;
      }

     // Skriv ut innehållet
    list_db(db, db_siz);
    edit_db(db, db_siz);
    list_db(db, db_siz);

  }
  return 0;
}

*/
//------------------------- 5.2 ------------------------------


// -----------------------5.3 ------------------------------


// Från utils.c


//Abstraherad funktion för alla ask_question funktioner. 
answer_t ask_question(char *question, check_func check, convert_func convert)
{
  int buf_siz = 255;
  char buf[buf_siz];
  int count = 0;
  do
    {
      if (count > 0)
	{
	  printf("Invalid input \'%s\', please try again.\n", buf);
	}
      printf("%s", question);
      read_string(buf, buf_siz);
      ++count;
    }
  while (!check(buf));
  return convert(buf);
}

//------------------ Is_number ------------------

bool is_number(char *str)
{
  int strl = strlen(str);

  if ((isdigit(str[0])) || (str[0]=='-')) 

    {
      for (int i = 1; i < strl; ++i)
        {
          if (!isdigit(str[i]))
        {
          return false;
        }
        }
      return true;
    }
  else
    {
      return false;
    }
}


//--------------------Del 1-----------------------
void clear_input_buffer()
{
 int c;
 do
        {
          c = getchar();
        }
 while (c != '\n' && c != EOF);
}

int ask_question_int(char *question) //Nya ask question in
{
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.i; //svaret som ett heltal
}


/* Gamla ask_question_int
int ask_question_int(char *question)
{
  int result = 0;
  int conversions = 0;
  do
    {
      printf("%s\n", question);
      conversions = scanf("%d", &result);
      clear_input_buffer();
      
    }
  while (conversions < 1);
  return result;
}

int main(void)
{
  int tal;
  tal = ask_question_int("Första talet:");
  printf("Du skrev '%d' \n", tal);

  tal = ask_question_int("Andra talet:");
  printf("Du skrev '%d'\n", tal);

  return 0;
}
*/

//-----------------Del 2------------------------------

int read_string(char *buff, int buff_siz)
{
  int counter = 0;
  char c;
  do
    {
      c = getchar();
      buff[counter] = c;
      ++counter;
    }
  while (counter < (buff_siz-1) && (c != '\n') && (c != EOF));
  buff[counter-1] = '\0';
  clear_input_buffer();
  return counter;
}

/*
int main (void)
{
  int buf_siz = 255;
  int read = 0;
  char buf[buf_siz];

  puts("Läs en sträng:");
  read = read_string(buf, buf_siz);
  printf("'%s' (%d tecken) \n", buf, read);

  return 0;
}
*/

//-----------------------del 3-----------------------------


bool not_empty(char *str) //fråga en ta!
{
  return (strlen(str) > 0);
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func) strdup).s;
}

/*
char *ask_question_string(char *question, char *buf, int buf_siz)
{
  int count = 0;
  do
    {
      printf("%s\n", question);
      count = read_string(buf, buf_siz);
    }
  while (count == 0);
  return buf;
}

int main()
{
  char buf[255];
  int buf_siz = 255;
  ask_question_string("What is your name?");
  return 0;
}
*/

// -------------- Lab 3 Print -----------------------------
int print(char *str)
{
  for (int i = 0; i < strlen(str); ++i)
    {
      putchar(str[i]);
    }
  return 0;
}

/*
char main(int argc, char *argv[])
{
  for (int i = 1; i < argc; ++i)
    {
      print(argv[i]);
    }
}
*/

// -------------- Lab 3 Println() -----------------------------

int println(char *str)
{
  print(str);
  printf("\n");
  return 0;
}

bool if_menu_choice(char *input) //kontroll funk. till ask_question_menu
{
  *input = toupper(*input);
  return (strlen(input) == 1 && (*input == 'R' || *input == 'E' || *input == 'U' || *input == 'L' || *input == 'Q' || *input == 'A'));
  
}

answer_t to_upper2(char *input)
{
  return (answer_t)
    {
      .c = toupper(*input)
	
    };
}

char ask_question_menu(char *question)
{
  return ask_question(question, if_menu_choice, (convert_func) to_upper2).c;
}

// ------------------------ Shelf som struct av hylla och antal

bool is_hylla(char *hylla)
{
  int nam_length = strlen(hylla);
  if (isalpha(hylla[0]))
    {
      for (int i = 1; i < nam_length; i++)
	{
	  if (!isdigit(hylla[i]))
	    {
	      return false;
	    }
	}
      return true;
    }
  return false;
}



char *ask_question_hylla(char *question)
{
  return ask_question(question, is_hylla, (convert_func) strdup).s;
}
