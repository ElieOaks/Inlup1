#ifndef __list_h__
#define __list_h__

#include <stdbool.h>

typedef struct list list_t;

typedef int L; // ändra till något som passar typ, hylla <3

list_t *new_list();
void list_append(list_t *list, L elem);
void list_prepend(list_t *list, L elem);
bool list_insert(list_t *list, int index, L elem);
bool list_remove(list_t *list, int index, L *elem);

L *list_get(list_t *list, int index);
L *list_first(list_t *list);
L *list_last(list_t *list);
int list_length(list_t *list);


#endif
