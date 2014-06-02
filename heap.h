#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>

// A simple min-heap implementation including dynamic sizing.

struct heap_struct;
typedef struct heap_struct heap_t;

heap_t *init_heap();
// Initialize an empty heap

void heap_delete(heap_t *heap);
// Delete a heap (may or may not be empty).

void heap_push(heap_t *heap, int priority, void *value);
// Push a new item onto the heap

void *heap_pop(heap_t *heap, bool *success, int *priority);
// Remove the min item from the heap.
// success. Must be supplied and will be set to if the heap is empty
// priority. Optional. Will be set to the priority of the removed item.


#endif // HEAP_H
