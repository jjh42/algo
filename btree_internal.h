#ifndef BTREE_INTERNAL_H
#define BTREE_INTERNAL_H

typedef struct {
  bool leaf;
  int n;
  int keys[]; // [2*t - 1]
  // Beyond the keys we store a corresponding list of children
  // btree_node_t *children[2*t];
  // void *values[2*t - 1];
} btree_node_t;


typedef struct
{
  map_t base;
  int t;
  btree_node_t *root;
} btree_t;


void* btree_get(btree_t *tree, int key, bool *found);
void btree_set(btree_t *tree, int key, void *value);
void btree_delete(btree_t *tree);


void ** get_values(btree_t *tree, btree_node_t *node);
btree_node_t ** get_children(btree_t *tree, btree_node_t *node);

#endif
