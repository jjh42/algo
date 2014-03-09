// Test runner for map implementations

#include <check.h>
#include <stdlib.h>

#include "map.h"

Suite *btree_suite();
Suite *hash_suite();
Suite *red_black_suite();


void test_basic_entries(map_t *m, int n_entries)
// Generic map test for inserting n_entries and reading them back
{
  bool found;

  for(int i = 0; i < n_entries; i++) {
    map_set(m, i, (void *) (long) (i + 1));
  }

  for(int i = 0; i < n_entries; i++) {
    int value = (int) map_get(m, i, &found);
    ck_assert(found);
    ck_assert_int_eq(value, i + 1);
  }

  map_get(m, -1, &found);
  ck_assert(!found);
}


void test_map_updates(map_t *m, int n_entries)
{
  for(int i = n_entries; i >= 0; i--) {
    map_set(m, i, (void *) (long) (i + 1));
  }
  for(int i = 0; i <= 100; i++) {
    map_set(m, i, (void *) (long) (i + 2));
  }

  for(int i = 0; i <= 100; i++) {
    bool found;
    int stored = (int) map_get(m, i, &found);
    ck_assert_int_eq(stored, i + 2);
    ck_assert(found);
  }
}


void test_suite(Suite *suite, bool *failed)
{
  SRunner *sr = srunner_create(suite);
  srunner_run_all(sr, CK_NORMAL);
  int n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  if (n_failed) {
    *failed = true;
  }
}

int main(int argc, char *argv[])
{
  bool failed = false;

  test_suite(btree_suite(), &failed);
  test_suite(hash_suite(), &failed);
  test_suite(red_black_suite(), &failed);
  return (!failed) ? EXIT_SUCCESS : EXIT_FAILURE;
}
