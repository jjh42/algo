#include "map.h"

void map_set(map_t *m, int key, void *value)
{
  m->vtable->set(m, key, value);
}

void *map_get(map_t *m, int key, bool *found)
{
  return m->vtable->get(m, key, found);
}

void map_free(map_t *m)
{
  return m->vtable->free(m);
}
