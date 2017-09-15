#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dblib.h"

#define QUIT 'A'
#define LIST 'H'
#define UNDO 'G'
#define EDIT 'R'
#define REMOVE 'T'
#define ADD 'L'

int event_loop (item_t *db, int db_siz)
{
while (true)
    {
      char answer = ask_question_menu("[L] Add item \n"
                                      "[T] Remove item \n"
                                      "[R] Edit item \n"
                                      "[G] Undo latest change \n"
                                      "[H] List the catalouge \n"
                                      "[A] Quit \n\n"
                                      "What would you like to do today? \n");

      switch (answer)
       {
        case ADD:
          add_item_to_db(db, &db_siz);
          break;
        case REMOVE:
          remove_item_from_db(db, &db_siz);
          break;
       case EDIT:
         edit_db(db, db_siz);
         break;
       case UNDO:
         printf("Not yet implemented! \n");
         break;
       case LIST:
         list_db(db, db_siz);
         break;
       case QUIT:
         printf("Cheers mate! \n");
         return 0;
         break;
         
       }
    }

  return 0;
}



int main(int argc, char *argv[])
{
  srand(time(NULL));
  char *array1[] = { "Laser", "Polka", "Extra"}; /// Lägg till!
  char *array2[] = { "smakande", "ordinär", "smart" }; /// Lägg till!
  char *array3[] = { "skruvdragare", "kamera", "fotbad" }; /// Lägg till!

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
        //item_t item = input_item();
	// db[db_siz] = item;
        //++db_siz;
      }
    }
    else
    {
      puts("Sorry, must have [1-16] items in database.");
      return 1; // Avslutar programmet!
    }

    for (int i = db_siz; i < 16; ++i)
      {
        char *name = magick(array1, array2, array3, 2); /// Lägg till storlek
        char *desc = magick(array1, array2, array3, 2); /// Lägg till storlek
        int price = random() % 200000;
        char shelf[] = { random() % ('Z'-'A') + 'A',
                         random() % 10 + '0',
                         random() % 10 + '0',
                         '\0' };
        item_t item = make_item(name, desc, price, shelf);

        db[db_siz] = item;
        ++db_siz;
      }
    /*
     // Skriv ut innehållet
     for (int i = 0; i < db_siz; ++i)
     {
       print_item(&db[i]);
       }
    */

    // Här nere händer det skit!

    //ask_question_menu("Choose an input: \n");
    event_loop(db, db_siz);

  }
  return 0;
}

