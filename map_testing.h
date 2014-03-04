/* Macros for generic map testing.
 */
#ifndef MAP_TESTING_H
#define MAP_TESTING_H

#include <check.h>
#include "map.h"
#include <assert.h>


void test_basic_entries(map_t *m, int n_entries);


#define MAP_TESTS(PREFIX, CORE_NAME, INIT)	   \
  START_TEST(PREFIX##test_map_create_destruct) \
  {\
    map_t *m = INIT;\
    map_free(m);\
  }\
  END_TEST\
  \
  START_TEST(PREFIX##test_map_basic) \
  {\
    map_t *m = INIT;\
	test_basic_entries(m, 100);\
	\
    map_free(m);\
  }\
  END_TEST\
  \
  TCase *PREFIX##_tests_core_create()			\
  {\
    TCase *tc_core = tcase_create(CORE_NAME);\
    tcase_add_test(tc_core, PREFIX##test_map_create_destruct);\
    tcase_add_test(tc_core, PREFIX##test_map_basic); \
	return tc_core;											  \
  }


#endif // MAP_TESTING_H
