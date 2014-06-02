#include <stdlib.h>
#include <assert.h>

#include "heap.h"

typedef struct { int priority; void *value; } heap_entry_t;

struct heap_struct
{
  unsigned int n_items;
  unsigned int allocated_space;
  heap_entry_t *entries;
};


heap_t *init_heap()
{
  heap_t *heap = (heap_t*) malloc(sizeof(heap_t));
  heap->n_items = 0;
  heap->allocated_space = 0;
  heap->entries = NULL;

  return heap;
}


void heap_delete(heap_t *heap)
{
  if(heap->entries) {
	free(heap->entries);
  }

  free(heap);
}


void ensure_space(heap_t *heap)
// Called prior to insertion in the heap
// Ensure that there is space in the heap
// for the new item - doubling the size of the
// allocated_space if needed.
{
  if(heap->n_items < heap->allocated_space) {
	// Space already
	return;
  }

  if (heap->allocated_space == 0) {
	assert(heap->entries == NULL);
	// This is the first allocation
	heap->entries = (heap_entry_t *) malloc(sizeof(heap_entry_t));
	heap->allocated_space = 1;
  }
  else {
	assert(heap->entries);
	unsigned int new_size = heap->allocated_space*2;
	heap->entries = (heap_entry_t *) realloc(heap->entries,
											 sizeof(heap_entry_t)*new_size);
	heap->allocated_space = new_size;
  }
}


// Heap is stored in the standard way of an array.
// Two helper functions for returning the parent and child
// indices for a node.
// Note that this may not be a valid index.
int parent(int i)
{
  return ((i + 1) & (~0x01))/2 - 1;
}

int left_child(int i)
{
  return 2*i + 1;
}

int right_child(int i)
{
  return 2*i + 2;
}


void swap(heap_t *heap, int i, int j)
// Helper function
// Swap items i and j in the heap
{
  heap_entry_t tmp = heap->entries[i];
  heap->entries[i] = heap->entries[j];
  heap->entries[j] = tmp;
}


void heap_push(heap_t *heap, int priority, void *value)
{
  ensure_space(heap);

  int i = heap->n_items;
  heap->entries[i].priority = priority;
  heap->entries[i].value = value;
  heap->n_items ++;

  // Up-heapify;
  while(i > 0) {
	int parent_i = parent(i);
	if(heap->entries[parent_i].priority > heap->entries[i].priority) {
	  // Swap to maintain parents being min of their children.
	  swap(heap, parent_i, i);
	  i = parent_i;
    }
	else {
	  break;
    }
  }
}

void *heap_pop(heap_t *heap, bool *success, int *priority)
{
  if(heap->n_items == 0) {
	*success = false;
	return NULL;
  }

  heap_entry_t min_item = heap->entries[0];
  if(heap->n_items > 1) {
	heap->entries[0] = heap->entries[heap->n_items - 1];
  }
  heap->n_items --;

  // Down-heapify
  int i = 0;
  while(i < heap->n_items) {
	int left_child_i = left_child(i);
	int right_child_i = right_child(i);
	int i_priority = heap->entries[i].priority;

	if(left_child_i >= heap->n_items) {
	  // No valid children
	  break;
	}

	int left_priority = heap->entries[left_child_i].priority;
	if(right_child_i >= heap->n_items) {
	  // No valid right child - only swap candidate is with the left child
	  if(left_priority < i_priority) {
		swap(heap, left_child_i, i);
		i = left_child_i;
		continue;
	  }
	  else {
		break;
	  }
    }

	int right_priority = heap->entries[right_child_i].priority;
	if((i_priority <= left_priority) && (i_priority <= right_priority)) {
	  // We already meet the heap order so stop
	  break;
	}

	// We need to swap with the max of the left or right child
	if(left_priority < right_priority) {
	  swap(heap, left_child_i, i);
	  i = left_child_i;
	}
	else {
	  swap(heap, right_child_i, i);
	  i = right_child_i;
	}
  }

  *success = true;
  *priority = min_item.priority;
  return min_item.value;
}
