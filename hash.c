#include <assert.h>
#include <stdlib.h>

#include "hash.h"
#include "hash_internal.h"

hash_entry_t hash_entry_unused;

const struct map_vtable_struct hash_vtable =
{
  .get = (map_get_t) hash_get,
  .set = (map_set_t) hash_set,
  .free = (map_free_t) hash_free
};


int calc_hash(hash_t *hash, int x)
// Calculate the hash of key of hash table h (result is always 0 < h(key) < table_ne
// Uses the multiply-shift mehtod (Dietzfelbinger et al., 1997)
// see https://en.wikipedia.org/wiki/Universal_hashing
{
  const int w = sizeof(int)*8;
  const int a = 0.6180339887*(1 << w);
  int M = hash->size_exponent;

  unsigned int hkey = ((unsigned int) (a*x)) >> (w-M);

  assert(hkey < (1 << hash->size_exponent));
  return hkey;
}


hash_entry_t *init_table(int size)
// Allocate space for and initialize an empty table with size entries
{
  assert(size > 0);
  hash_entry_t *table = malloc(sizeof(hash_entry_t)*size);
  for(int i = 0; i < size; i++)
  {
	table[i].next = &hash_entry_unused;
	table[i].key = 0;
	table[i].value = 0;
  }

  return table;
}


void transfer_from_old_table(hash_t *hash, int old_table_len, hash_entry_t *old_table)
{
  for(int i = 0; i < old_table_len; i++) {
	if(old_table[i].next != &hash_entry_unused) {
	  hash_entry_t *entry = &old_table[i];
	  while(entry) {
		hash_set(hash, entry->key, entry->value);
		entry = entry->next;
	  }
    }
  }
}


void expand_table(hash_t *hash)
{
  int old_table_len = (1 << hash->size_exponent);
  hash_entry_t *old_table = hash->table;

  hash->size_exponent++;
  hash->table = init_table(1 << hash->size_exponent);

  transfer_from_old_table(hash, old_table_len, old_table);
}

void examine_load_factor(hash_t *hash)
// Examine if the hash table needs to be expanded
{
  int table_len = (1 << hash->size_exponent);
  int inv_load_factor = table_len / (hash->n_items + 1); // The plus 1 is to avoid
                                                         //a divide by zero

  if(inv_load_factor < 2) {
	expand_table(hash);
  }
}

void hash_set(hash_t *hash, int key, void *value)
{
  examine_load_factor(hash);

  int hkey = calc_hash(hash, key);

  if(hash->table[hkey].next == &hash_entry_unused) {
	// We are the first entry at this hash
	hash->table[hkey].next = NULL;
	hash->table[hkey].value = value;
	hash->table[hkey].key = key;
	hash->n_items++;
	return;
  }

  // Not the first entry at this hash
  // Walk the chain and see if this key already exists
  // otherwise append at the end
  hash_entry_t *entry = &hash->table[hkey];
  while(entry) {
	if(entry->key == key) {
	  // Update existing entry
	  entry->value = value;
	  return;
    }
	entry = entry->next;
  }

  // Insert as the second entry in the list (this just happens
  // to be a convenient location for insertion.
  hash_entry_t *new_entry = malloc(sizeof(hash_entry_t));
  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = hash->table[hkey].next;
  hash->table[hkey].next = new_entry;
  hash->n_items++;

}


void *hash_get(hash_t *hash, int key, bool *found)
{
  int hkey = calc_hash(hash, key);

  if(hash->table[hkey].next == &hash_entry_unused) {
	goto not_found;
  }

  // There is a hash chain at this entry. So follow the chain
  // and see if this key is in it.
  hash_entry_t *entry = &hash->table[hkey];
  while(entry) {
	if(entry->key == key) {
	  *found = true;
	  return entry->value;
    }
	entry = entry->next;
  }

not_found:
  *found = false;
  return 0;
}


void hash_free(hash_t *hash)
{
  // Free each of the linked lists
  int table_len = 1 << hash->size_exponent;
  hash_entry_t *table = hash->table;

  for(int i = 0; i < table_len; i++) {
	if(table[i].next != &hash_entry_unused) {
	  hash_entry_t *next = table[i].next;
	  while(next) {
		hash_entry_t *temp = next->next;
		free(next);
		next = temp;
      }
    }
  }
  free(hash->table);
  free(hash);
}


map_t *init_hash()
{
  hash_t *hash = malloc(sizeof(hash_t));
  const int initial_exponent = 2; // Table sizes are always powers of 2
  hash_entry_t *table = init_table(1 << initial_exponent);
  hash->table = table;
  hash->n_items = 0;
  hash->size_exponent = initial_exponent;
  hash->base.vtable = &hash_vtable;

  return (map_t*) hash;
}
