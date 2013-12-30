#include <check.h>
#include <stdlib.h>


#include "median.h"


START_TEST(test_insertion_sort)
{
  int a[] = {3, 2, 1};
  insertion_sort(a, 3);
  for(int i = 0; i < 2; i++) {
    ck_assert(a[i] <= a[i+1]);
  }
}
END_TEST


START_TEST(test_partition)
{
  int a[] = {12, 9, 3, 4, 11, 10, 7};
  partition(a, 7, 7);
  for(int i = 0; i < 2; i++) {
	ck_assert(a[i] < 7);
  }
  ck_assert(a[2] == 7);
  for(int i = 3; i < 7; i++) {
	ck_assert(a[i] > 7);
  }
}
END_TEST


START_TEST(test_select)
{
  int a[] = {12, 9, 3, 4, 11, 10, 7};
  int median = select(a, 3, 7);
  ck_assert_int_eq(median, 9);
}
END_TEST


Suite *median_suite()
{
  Suite *s = suite_create("Median");
  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_insertion_sort);
  tcase_add_test(tc_core, test_partition);
  tcase_add_test(tc_core, test_select);
  suite_add_tcase(s, tc_core);

  return s;
}


int main()
{
  Suite *s = median_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (n_failed == 0)? EXIT_SUCCESS : EXIT_FAILURE;
}

