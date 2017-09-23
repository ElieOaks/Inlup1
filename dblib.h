#ifndef __DBLIB_H__
#define __DBLIB_H__
#include <stdbool.h>

extern char *strdup(const char *); // för ask_question_string
typedef union
{
  int i;
  float f;
  char c; //för ask_question_menu
  char *s;
} answer_t;

struct shelf
{
  char *Hylla;
  int Antal; 
};

typedef struct goods goods_t;
typedef struct shelf shelf_t;
typedef bool(*check_func)(char *); // kontroll funktion
typedef answer_t(*convert_func)(char *); //converterings funktion

int read_string(char *buf, int buf_siz);

answer_t to_upper2(char *input);
answer_t ask_question(char *question, check_func check, convert_func convert);
int ask_question_int(char *question);
char *ask_question_string(char *question);
char *ask_question_hylla(char *question);
char ask_question_menu(char *question);

bool is_hylla(char *hylla);
bool is_number(char *str);
bool not_empty(char *str);
bool if_menu_choice(char *input);

int println(char *str);
int print(char *str);
#endif
