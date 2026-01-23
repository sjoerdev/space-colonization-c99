#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct List
{
    void *data;         // pointer to allocated memory
    int element_size;   // size of each element
    int size;           // number of elements currently in list
    int capacity;       // total allocated capacity
} List;

void list_init(List* list, int element_size);
void list_free(List* list);
void list_add(List* list, void* element);
void list_remove(List* list, int index);
void list_clear(List *list);
void* list_get(List* list, int index);
int list_size(List* list);
void list_remove_list(List* list, List* to_remove);

#endif // LIST_H