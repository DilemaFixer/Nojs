#include "arr.h"
#include <string.h>

arr_t *new_arr(size_t initial_capacity) {
  if (initial_capacity == 0) {
    initial_capacity = 4;
  }

  arr_t *arr = malloc(sizeof(arr_t));
  if (!arr) {
    return NULL;
  }

  arr->items = malloc(initial_capacity * sizeof(void *));
  if (!arr->items) {
    free(arr);
    return NULL;
  }

  arr->count = 0;
  arr->capacity = initial_capacity;
  return arr;
}

void free_arr(arr_t *arr) {
  if (!arr)
    return;

  if (arr->items) {
    free(arr->items);
  }
  free(arr);
}

bool arr_push(arr_t *arr, void *element) {
  if (!arr)
    return false;

  if (arr->count >= arr->capacity) {
    size_t new_capacity = arr->capacity * 2;
    if (!arr_resize(arr, new_capacity)) {
      return false;
    }
  }

  arr->items[arr->count++] = element;
  return true;
}

void *arr_get(arr_t *arr, size_t index) {
  if (!arr || index >= arr->count) {
    return NULL;
  }
  return arr->items[index];
}

bool arr_set(arr_t *arr, size_t index, void *element) {
  if (!arr || index >= arr->count) {
    return false;
  }
  arr->items[index] = element;
  return true;
}

bool arr_remove(arr_t *arr, size_t index) {
  if (!arr || index >= arr->count) {
    return false;
  }

  for (size_t i = index; i < arr->count - 1; i++) {
    arr->items[i] = arr->items[i + 1];
  }

  arr->count--;
  return true;
}

size_t arr_size(arr_t *arr) { return arr ? arr->count : 0; }

bool arr_resize(arr_t *arr, size_t new_capacity) {
  if (!arr || new_capacity < arr->count) {
    return false;
  }

  void **new_items = realloc(arr->items, new_capacity * sizeof(void *));
  if (!new_items) {
    return false;
  }

  arr->items = new_items;
  arr->capacity = new_capacity;
  return true;
}
