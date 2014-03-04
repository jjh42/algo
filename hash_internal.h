#ifndef HASH_INTERNAL_H
#define HASH_INTERNAL_H

#include "hash.h"

struct hash_entry_struct;
typedef struct hash_entry_struct hash_entry_t;

struct hash_entry_struct
{
  hash_entry_t *next;
  int key;
  void *value;
};


typedef struct
{
  map_t base;
  int n_items;
  int size_exponent;
  hash_entry_t *table;
} hash_t;


void* hash_get(hash_t *h, int key, bool *found);
void hash_set(hash_t *h, int key, void *value);
void hash_free(hash_t *h);

// Sentinel used to point to indicate that a hash entry is unused
// (NULL indicates this entry is valid but unchained)
extern hash_entry_t hash_entry_unused;

#endif // HASH_INTERNAL_H
