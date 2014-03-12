#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "btree.h"
#include "hash.h"
#include "red_black.h"


map_t *init_map(const char *choice)
{
  if(strncmp(choice, "btree", 5) == 0) {
	int n = atoi(&choice[5]);
	return init_btree(n);
  }
  else if (strcmp(choice, "hash") == 0) {
	return init_hash();
  }
  else if (strcmp(choice, "red_black") == 0) {
	return init_red_black();
  }
  else {
	fprintf(stderr, "Invalid choice %s\n", choice);
	return NULL;
  }
}


void benchmark(map_t *map, int n)
// Insert and lookup n items
{
  int *items = malloc(sizeof(int)*n);
  for(int i = 0; i < n; i++) {
	items[i] = rand();
	map_set(map, items[i], &items[i]);
  }

  bool found;

  for(int i = 0; i < n; i++) {
	int *item_ptr = map_get(map, items[i], &found);
	assert(found);
	assert(item_ptr == &items[i]);
  }

  free(items);
}

int main(int argc, char *argv[])
{
  assert(argc == 3);

  char *map_choice = argv[1];
  int n_entries = atoi(argv[2]);
  assert(n_entries > 0);

  map_t *m = init_map(map_choice);
  if(!m) {
	return -1;
  }

  benchmark(m, n_entries);

  map_free(m);
  return 0;
}
