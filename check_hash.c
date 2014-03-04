#include "hash.h"
#include "hash_internal.h"
#include "map_testing.h"

MAP_TESTS(hash, "hash", init_hash());


int chain_len(hash_entry_t *next) {
  int len = 0;
  while(next) {
	len++;
	next = next->next;
  }

  return len;
}


float calc_avg_collisions(hash_t *hash)
// Calculate the average number of collisions per entry in the hash table
// (so ideally, would be 0 for no collisions.
{
  int n_collisions = 0;

  hash_entry_t *table = hash->table;
  int table_len = (1 << hash->size_exponent);
  for(int i = 0; i < table_len; i++) {
	if(table[i].next != &hash_entry_unused) {
	  n_collisions += chain_len(&table[i]);
    }
  }

  return ((float) n_collisions)/((float) table_len);
}


START_TEST(test_hash_collisions)
// Check that the number of collisions stays within reasonable bounds.
// This has the side-effect of checking that resizing is occurring.
{
  const int n_entries = 100;
  hash_t *hash = (hash_t *) init_hash();

  for(int i = 0; i < n_entries; i++) {
	map_set((map_t*) hash, i, (void *) (long) i);
  }

  float avg_collisions = calc_avg_collisions(hash);
  assert(avg_collisions < 1.0);
}
END_TEST


Suite *hash_suite()
{
  Suite *s = suite_create("hash");
  TCase *hash_core = hash_tests_core_create();
  TCase *hash_extras = tcase_create("hash_extras");
  tcase_add_test(hash_extras, test_hash_collisions);

  suite_add_tcase(s, hash_extras);
  suite_add_tcase(s, hash_core);

  return s;
}
