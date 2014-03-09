#include <check.h>
#include <stdlib.h>


#include "map_testing.h"

#include "btree.h"
#include "btree_internal.h"


START_TEST(test_btree_init)
{
  btree_t *tree = (btree_t*) init_btree(3);

  ck_assert_ptr_eq(tree->base.vtable->get, btree_get);
  ck_assert_ptr_eq(tree->base.vtable->set, btree_set);

  map_free((map_t *) tree);
}
END_TEST


START_TEST(test_btree_one_entry)
{
  btree_t *tree = (btree_t*) init_btree(3);
  map_set((map_t*) tree, 1, (void *) 2);

  ck_assert_int_eq(tree->root->n, 1);
  ck_assert_int_eq(tree->root->keys[0], 1);
  ck_assert_int_eq((int) get_values(tree, tree->root)[0], 2);
  ck_assert(tree->root->leaf);

  bool found;
  ck_assert_int_eq((int) map_get((map_t*)tree, 1, &found), 2);
  ck_assert(found);

  map_get((map_t*) tree, 5, &found);
  ck_assert(!found);

  map_free((map_t*) tree);

}
END_TEST


START_TEST(test_btree_more_entries)
{
  btree_t *tree = (btree_t*) init_btree(3);
  for(int i = 0; i < 5; i++) {
    map_set((map_t*) tree, i, (void *) (long) (i + 1));
  }

  ck_assert(tree->root->leaf);

  for(int i = 0; i < 5; i++) {
    bool found;
    int m = (int) map_get((map_t*) tree, i, &found);
    ck_assert_int_eq(m, i + 1);
    ck_assert(found);
  }
}
END_TEST


START_TEST(test_btree_more_entries_reversed)
{
  btree_t *tree = (btree_t*) init_btree(3);
  for(int i = 25; i >= 0; i--) {
    map_set((map_t*) tree, i, (void *) (long) (i + 1));
  }

  for(int i = 0; i <= 25; i++) {
    bool found;
    int m = (int) map_get((map_t*) tree, i, &found);
    ck_assert_int_eq(m, i + 1);
    ck_assert(found);
  }

  map_free((map_t*) tree);
}
END_TEST


MAP_TESTS(btree_map2, "btree_map2", init_btree(2));
MAP_TESTS(btree_map3, "btree_map3", init_btree(3));
MAP_TESTS(btree_map5, "btree_map5", init_btree(5));


Suite *btree_suite()
{
  Suite *s = suite_create("Btree");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_btree_init);
  tcase_add_test(tc_core, test_btree_one_entry);
  tcase_add_test(tc_core, test_btree_more_entries);
  tcase_add_test(tc_core, test_btree_more_entries_reversed);

  TCase *map_core3 = btree_map3_tests_core_create();
  TCase *map_core5 = btree_map5_tests_core_create();

  suite_add_tcase(s, tc_core);
  suite_add_tcase(s, map_core3);
  suite_add_tcase(s, map_core5);

  return s;
}
