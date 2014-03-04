#ifndef MAP_H
#define MAP_H

#include <stdbool.h>


struct map_vtable_struct;


// The generic map data structure
// maps int -> void *
// User is responsible for handling memory management of
// data (if they are pointers)


typedef struct {
  struct map_vtable_struct const *vtable;
} map_t;

typedef void (*map_set_t) (map_t *, int, void *);
typedef void *(*map_get_t) (map_t *, int, bool *);
typedef void (*map_free_t)(map_t *);


typedef struct map_vtable_struct 
{
  map_set_t set;
  map_get_t get;
  map_free_t free;
} map_vtable_t;


void map_set(map_t *m, int key, void *value);
void *map_get(map_t *m, int key, bool *found);
void map_free(map_t *m);

#endif // MAP_H
