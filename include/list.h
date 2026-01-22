#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    void *data;         // pointer to allocated memory
    int element_size;   // size of each element
    int size;           // number of elements currently in list
    int capacity;       // total allocated capacity
} List;

// init list (element_size is the sizeof of the type)
void list_init(List* list, int element_size)
{
    list->data = NULL;
    list->element_size = element_size;
    list->size = 0;
    list->capacity = 0;
}

// free list memory
void list_free(List* list)
{
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

// add element to list
void list_add(List* list, void* element)
{
    if (list->size >= list->capacity)
    {
        list->capacity = list->capacity ? list->capacity * 2 : 4;
        list->data = realloc(list->data, list->capacity * list->element_size);
        if (!list->data)
        {
            perror("realloc failed");
            exit(EXIT_FAILURE);
        }
    }
    memcpy((char*)list->data + list->size * list->element_size, element, list->element_size);
    list->size++;
}

// remove element at index
void list_remove(List* list, int index)
{
    if (index >= list->size || index < 0) return; // out of bounds
    if (index < list->size - 1)
    {
        memmove((char*)list->data + index * list->element_size,
                (char*)list->data + (index + 1) * list->element_size,
                (list->size - index - 1) * list->element_size);
    }
    list->size = list->size - 1;
}

// clear list (keeps allocated memory)
void list_clear(List *list)
{
    list->size = 0;
}

// access element by index
void* list_get(List* list, int index)
{
    if (index >= list->size || index < 0)
    {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return (char*)list->data + index * list->element_size;
}

// get list length
int list_size(List* list)
{
    return list->size;
}

void list_remove_list(List* list, List* to_remove)
{
    // remove the elements in the to_remove list from the main list
    for (int i = list_size(list) - 1; i >= 0; i--)  // go backwards to avoid shifting
    {
        void* node = list_get(list, i);
        
        // check if node is in toRemove
        for (int j = 0; j < list_size(to_remove); j++)
        {
            if (node == list_get(to_remove, j)) // comparing pointers
            {
                list_remove(list, i);
                break;
            }
        }
    }
}

#endif // LIST_H