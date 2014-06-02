#include <check.h>
#include <stdlib.h>
#include "heap.h"


START_TEST(test_simple_heap_example)
// Build a heap and check we can pop entries in the
// expected order.
{
  const int n_entries = 400;
  heap_t *heap = init_heap();

  for(int i = n_entries - 1; i >= 0; i--) {
	heap_push(heap, i, (void *) (long) i + 1);
  }

  for(int i = 0; i <= n_entries - 1; i++) {
	bool success;
	int priority;
	int result = (int) heap_pop(heap, &success, &priority);
	ck_assert(success);
	ck_assert_int_eq(priority, i);
	ck_assert_int_eq(result, i + 1);
  }

  bool success;
  heap_pop(heap, &success, NULL);
  ck_assert(!success);

  heap_delete(heap);
}
END_TEST


Suite *heap_suite()
{
  Suite *s = suite_create("heap");
  TCase *heap_tc = tcase_create("heap");
  tcase_add_test(heap_tc, test_simple_heap_example);
  suite_add_tcase(s, heap_tc);

  return s;
}

int main()
{
  Suite *s = heap_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (n_failed == 0)? EXIT_SUCCESS : EXIT_FAILURE;
}
