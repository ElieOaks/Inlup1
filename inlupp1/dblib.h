#ifndef __DBLIB_H__
#define __DBLIB_H__
#include <stdbool.h>

extern char *strdup(const char *);

typedef union {
  int i;
  char *s;
  char c;
} answer_t;

struct item {
  char *name;
  char *desc; 
  int price;
  char *shelf;
};

typedef struct item item_t;

typedef bool(*check_func)(char *);

typedef answer_t(*convert_func)(char *);

/*ask_question skit*/

answer_t ask_question(char *question, check_func check, convert_func convert);

int ask_question_int(char *question);

char *ask_question_shelf(char *question);

char *ask_question_string(char *question);

char ask_question_menu(char *question);

int read_string(char *buf, int buf_siz); // DEN BUGGAR

answer_t toupper2(char *str);

/*Checkfunktioner*/

bool is_valid_input(char *input); // samma som Joys if_menu_choice

bool is_shelf (char *shelf);

bool is_number (char *str);

bool not_empty(char *str);

/*Databasfunktioner*/

void print_item(item_t *item);

item_t make_item(char *name, char *desc, int price, char* shelf);

item_t input_item(void);

char *magick (char *array1[], char *array2[], char *array3[], int length);

void add_item_to_db(item_t *db, int *db_siz);

void list_db (item_t *db, int db_siz);

void remove_item_from_db(item_t *db, int *db_siz);

void edit_db (item_t *db, int db_siz);

#endif
