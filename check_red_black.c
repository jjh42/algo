#include <stdlib.h>

#include "red_black_internal.h"
#include "map_testing.h"


MAP_TESTS(red_black, "red_black", init_red_black ());


int max(int a, int b)
{
  if(a > b) {
	return a;
  }
  else {
	return b;
  }
}


int node_depth(red_black_node_t *node)
// Determine the maximum depth from this node
{
  if(node == &null_node) {
	return 0;
  }

  return 1 + max(node_depth(node->left), node_depth(node->right));
}


START_TEST(test_red_black_balancing)
{
  red_black_t *tree = (red_black_t*) init_red_black();
  const int n_entries = 256;

  for(int i = 0; i < n_entries; i++) {
	int key = rand();
    map_set((map_t*) tree, key, (void *) (long) (i + 1));
  }

  ck_assert_int_lt(node_depth(tree->root), 8*2);

  map_free((map_t*) tree);
}
END_TEST


Suite *red_black_suite()
{
  Suite *s = suite_create("red_black");
  TCase *red_black_core = red_black_tests_core_create();
  TCase *red_black_extras = tcase_create("red_black_extras");
  tcase_add_test(red_black_extras, test_red_black_balancing);

  suite_add_tcase(s, red_black_extras);
  suite_add_tcase(s, red_black_core);

  return s;
}
