#ifndef RED_BLACK_INTERNAL_H
#define RED_BLACK_INTERNAL_H

#include <stdbool.h>

#include "red_black.h"

enum red_black_color { COLOR_RED, COLOR_BLACK };

struct red_black_struct;
typedef struct red_black_struct {
  int key;
  enum red_black_color color;
  struct red_black_struct *parent;
  struct red_black_struct *left;
  struct red_black_struct *right;
  void *value;
} red_black_node_t;


typedef struct {
  map_t base;
  red_black_node_t *root;
} red_black_t;


void* red_black_get(red_black_t *tree, int key, bool *found);
void red_black_set(red_black_t *tree, int key, void *value);
void red_black_free(red_black_t *tree);

extern red_black_node_t null_node;

#endif // RED_BLACK_INTERNAL_H
