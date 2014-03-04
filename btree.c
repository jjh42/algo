#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "btree.h"
#include "btree_internal.h"


const struct map_vtable_struct btree_vtable =
{
  .get = (map_get_t) btree_get,
  .set = (map_set_t) btree_set,
  .free = (map_free_t) btree_free
};


void free_node(btree_t *tree, btree_node_t *node)
{
  if (! node->leaf) {
	for(int i = 0; i <= node->n; i++) {
	  free_node(tree, get_children(tree, node)[i]);
	}
  }
  free(node);
}

void btree_free(btree_t *tree)
{
  free_node(tree, tree->root);
  free(tree);
}


btree_node_t* alloc_node(btree_t *tree) {
  // Allocate space and initialize a new empty node.
  int t = tree->t;
  btree_node_t *node = malloc(sizeof(btree_node_t) + (2*t - 1)*sizeof(int)
							  + 2*t*sizeof(btree_node_t*) + (2*t - 1)*sizeof(void*));
  node->leaf = true;
  node->n = 0;

  return node;
}


void ** get_values(btree_t *tree, btree_node_t *node)
// Return a pointer ot the values array of node
{
  char *values = (char *)node;
  int t = tree->t;
  values += sizeof(btree_node_t) + (2*t - 1)*sizeof(int) + 2*t*sizeof(void *);
  return (void **) values;
}


btree_node_t ** get_children(btree_t *tree, btree_node_t *node)
// Return a pointer to the children array of node.
{
  char *child = (char *) node;
  int t = tree->t;
  child += sizeof(btree_node_t) + (2*t - 1)*sizeof(int);
  return (btree_node_t **) child;
}


map_t *init_btree(int t)
{
  btree_t *tree = malloc(sizeof(btree_t));
  tree->t = t;
  assert(t >= 1);
  tree->root = alloc_node(tree);
  tree->base.vtable = &btree_vtable;
  return (map_t*) tree;
}


void *search_node(btree_t *tree, btree_node_t *node, int key,
				  bool *found)
// Recursively search the nodes, returning 0 if no
// match is found.
{
  int i;
  for(i = 0; (i < (node->n)) && (key > node->keys[i]); i++);

  if((i < node->n) && ( key == node->keys[i] )) {
	// Found a match
	*found = true;
	return get_values(tree, node)[i];
  }
  else if ( node->leaf ) {
	*found = false;
	return 0;
  }
  else {
	return search_node(tree, get_children(tree, node)[i], key, found);
  }

}


void split_child(btree_t *tree, btree_node_t *x, int i)
//
//        (x)                             (x)
//         |                               |
//   ...a  b  c ...    ->          ... a  u  b  c ...
//       |                               |  |
//      (y)                             (y) (z)
//       |                               |   |
//   p q r t u w y z                  p..t   w..z
{
  btree_node_t *z = alloc_node(tree);
  btree_node_t *y = get_children(tree, x)[i];
  int t = tree->t;

  assert(y->n == (2*t - 1));
  assert(x->n < (2*t - 1));

  z->leaf = y->leaf;
  z->n = t - 1;
  for(int j = 0; j < (t - 1); j++) {
    z->keys[j] = y->keys[j + t];
	get_values(tree, z)[j] = get_values(tree, y)[j + t];
  }

  if (! y->leaf) {
	for(int j = 0; j < t; j++) {
	  get_children(tree, z)[j] = get_children(tree, y)[j + t];
	}
  }
  y->n = t-1;

  // y has been split into two nodes. now insert the new node z in the
  // correct position in the parent x and insert the median key in the correct
  // position in the parent node.
  for(int j = (x->n + 1); j > i; j--) {
	get_children(tree, x)[j] = get_children(tree, x)[j - 1];
  }
  get_children(tree, x)[i + 1] = z;

  for(int j = x->n; j > i; j--) {
	x->keys[j] = x->keys[j - 1];
	get_values(tree, x)[j] = get_values(tree, x)[j - 1];
  }
  x->keys[i] = y->keys[t - 1];
  get_values(tree, x)[i] = get_values(tree, y)[t - 1];
  x->n ++;
}


void insert_in_leaf_node(btree_t *tree, btree_node_t *x, int key, void *value)
{
  // First check if the key already exists
  for(int k = 0; k < x->n; k++) {
	if (x->keys[k] == key) {
	  get_values(tree, x)[k] = value;
	  return;
    }
  }

  int i = x->n - 1;
  for(; (i >= 0) && (key < x->keys[i]); i--) {
	x->keys[i+1] = x->keys[i];
	get_values(tree, x)[i+1] = get_values(tree, x)[i];
  }

  x->keys[i+1] = key;
  get_values(tree, x)[i+1] = value;
  x->n ++;
}

void insert_nonfull(btree_t *tree, btree_node_t *x, int key, void *value)
{
  if( x-> leaf ) {
	insert_in_leaf_node(tree, x, key, value);
    return;
  }

  int i = x->n - 1;

  for(; (i >= 0) && (key < x->keys[i]); i--);

  if((i >= 0) && (x->keys[i] == key)) {
	// Update existing value
	get_values(tree, x)[i] = value;
    return;
  }

  i++;

  btree_node_t *child = get_children(tree, x)[i];
  if(child->n == (2*tree->t - 1)) {
	split_child(tree, x, i);
	if (key > x->keys[i]) {
	  child = get_children(tree, x)[i + 1];
	}
  }
  insert_nonfull(tree, child, key, value);

}


void btree_set(btree_t *tree, int key, void *value)
{
  btree_node_t *root = tree->root;
  int t = tree->t;

  if ( root->n == (2*t - 1) ) {
	// The root node is full
	btree_node_t *s = alloc_node(tree);
	tree->root = s;
	s->leaf = false;
	s->n = 0;
	get_children(tree, s)[0] = root;
	split_child(tree, s, 0);
	insert_nonfull(tree, s, key, value);
  } else {
	insert_nonfull(tree, root, key, value);
  }
}


void* btree_get(btree_t *tree, int key, bool *found) {
  return search_node(tree, tree->root, key, found);
}

