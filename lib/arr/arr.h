#ifndef ARR_H
#define ARR_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    void** items;
    size_t count;
    size_t capacity;
} arr_t;

arr_t* new_arr(size_t initial_capacity);
void free_arr(arr_t* arr);
bool arr_push(arr_t* arr, void* element);
void* arr_get(arr_t* arr, size_t index);
bool arr_set(arr_t* arr, size_t index, void* element);
bool arr_remove(arr_t* arr, size_t index);
size_t arr_size(arr_t* arr);
bool arr_resize(arr_t* arr, size_t new_capacity);

#endif
