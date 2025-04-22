#include "queue.h"
#include <stdbool.h>

queue_t *new_queue(size_t capacity) {
  if (capacity == 0)
    lfatal("Cannot create queue with zero capacity");
  queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
  if (!queue)
    lfatal("Failed to allocate memory for queue");
  queue->q = (void **)malloc(sizeof(void *) * capacity);
  if (!queue->q) {
    free(queue);
    lfatal("Failed to allocate memory for queue elements");
  }
  queue->capacity = capacity;
  queue->count = 0;
  queue->front = 0;
  queue->rear = 0;
  return queue;
}

void free_queue(queue_t *queue) {
  if (!queue)
    return;
  if (queue->q)
    free(queue->q);
  free(queue);
}

void resize_queue(queue_t *queue) {
  if (!queue || !queue->q)
    lfatal("Cannot resize an invalid queue");
  size_t new_capacity = queue->capacity * 2;
  void **new_q = (void **)malloc(sizeof(void *) * new_capacity);
  if (!new_q)
    lfatal("Failed to allocate memory for queue resize");
  size_t j = 0;
  for (size_t i = queue->front; i != queue->rear;
       i = (i + 1) % queue->capacity) {
    new_q[j++] = queue->q[i];
  }
  free(queue->q);
  queue->q = new_q;
  queue->front = 0;
  queue->rear = queue->count;
  queue->capacity = new_capacity;
}

void enqueue(queue_t *queue, void *item) {
  if (!queue || !queue->q)
    lfatal("Cannot enqueue to an invalid queue");
  if (!item)
    lfatal("Cannot enqueue NULL item");
  if (queue->count == queue->capacity) {
    resize_queue(queue);
  }
  queue->q[queue->rear] = item;
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->count++;
}

void *dequeue(queue_t *queue) {
  if (!queue || !queue->q)
    lfatal("Cannot dequeue from an invalid queue");
  if (queue->count == 0)
    lfatal("Cannot dequeue from an empty queue");
  void *item = queue->q[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->count--;
  return item;
}

void *peek(queue_t *queue) {
  if (!queue || !queue->q)
    lfatal("Cannot peek into an invalid queue");
  if (queue->count == 0)
    lfatal("Cannot peek into an empty queue");
  return queue->q[queue->front];
}

bool is_queue_empty(queue_t *queue) {
  if (!queue)
    return true;
  return queue->count == 0;
}

size_t queue_size(queue_t *queue) {
  if (!queue)
    return 0;
  return queue->count;
}
