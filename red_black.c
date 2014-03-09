#include <assert.h>
#include <stdlib.h>

#include "red_black_internal.h"


red_black_node_t null_node = {.color = COLOR_BLACK};

const struct map_vtable_struct red_black_vtable =
{
  .get = (map_get_t) red_black_get,
  .set = (map_set_t) red_black_set,
  .free = (map_free_t) red_black_free
};

map_t *init_red_black()
{
  red_black_t *tree = malloc(sizeof(red_black_t));
  tree->base.vtable = &red_black_vtable;
  tree->root = &null_node;

  return (map_t*) tree;
}


void red_black_free_node(red_black_node_t *node)
// Depth first free the nodes
{
  if(node != &null_node) {
	red_black_free_node(node->left);
	red_black_free_node(node->right);

	free(node);
  }
}


void red_black_free(red_black_t *tree)
{
  red_black_free_node(tree->root);
  free(tree);
}

void left_rotate(red_black_t *tree, red_black_node_t *x)
//
//   ( x )                     ( y )
//    / \           ->          / \
// (a)  (y)                   (x) (c) 
//      / \                   / \
//     (b) (c)               (a) (b)
{
  red_black_node_t *y = x->right;
  assert(y != &null_node);

  x->right = y->left;
  if(y->left != &null_node) {
	y->left->parent = x;
  }
  y->parent = x->parent;

  // Either x is the root of tree,
  // or x is the left or right node of its
  // parent. Update this to location to point to y.
  if (x->parent == &null_node) {
	tree->root = y;
  }
  else if ( x == x->parent->left) {
	x->parent->left = y;
  }
  else {
	assert(x->parent->right == x); 
	x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}


void right_rotate(red_black_t *tree, red_black_node_t *y)
//
//   ( x )                     ( y )
//    / \           <-          / \
// (a)  (y)                   (x) (c) 
//      / \                   / \
//     (b) (c)               (a) (b)
{
  red_black_node_t *x = y->left;
  assert(x != &null_node);

  y->left = x->right;
  if(x->right != &null_node) {
	x->right->parent = y;
  }
  x->parent = y->parent;

  // Either y is the root of the tree or the left
  // or right node of its parent. Update the pointer from the parent
  // to point to x
  if (y->parent == &null_node) {
	tree->root = x;
  }
  else if (y->parent->left == y) {
	y->parent->left = x;
  }
  else {
	assert(y->parent->right == y);
	y->parent->right = x;
  }

  x->right = y;
  y->parent = x;
}


void red_black_insert_fixup(red_black_t *tree, red_black_node_t *z)
// Maintain the balance of the red-black tree (see Intro to Algorithms, pg 316)
{
  while(z->parent->color == COLOR_RED) {
	assert(z->color == COLOR_RED);

	if(z->parent == z->parent->parent->left) {
	  red_black_node_t *y = z->parent->parent->right;
	  if(y->color == COLOR_RED) {
		z->parent->color = COLOR_BLACK;
		y->color = COLOR_BLACK;
		z->parent->parent->color = COLOR_RED;
		z = z->parent->parent;
	  }
	  else {
		if(z == z->parent->right) {
		  z = z->parent;
		  left_rotate(tree, z);
		}
		z->parent->color = COLOR_BLACK;
		z->parent->parent->color = COLOR_RED;
		right_rotate(tree, z->parent->parent);
	  }
    }
	else {
	  red_black_node_t *y = z->parent->parent->left;
	  if(y->color == COLOR_RED) {
		z->parent->color = COLOR_BLACK;
		y->color = COLOR_BLACK;
		z->parent->parent->color = COLOR_RED;
		z = z->parent->parent;
	  }
	  else {
		if(z == z->parent->left) {
		  z = z->parent;
		  right_rotate(tree, z);
		}
		z->parent->color = COLOR_BLACK;
		z->parent->parent->color = COLOR_RED;
	    left_rotate(tree, z->parent->parent);
	  }
    }
  }

  tree->root->color = COLOR_BLACK;
}

void red_black_set(red_black_t *tree, int key, void *value)
{
  red_black_node_t *y = &null_node;
  red_black_node_t *x = tree->root;
  while(x != &null_node) {
	y = x;
	if(key < x->key) {
	  x = x->left;
    }
	else if(key > x->key) {
	  x = x->right;
    }
	else {
	  // Update rather than an insertion
	  assert(key == x->key);
	  x->value = value;
	  return;
    }
  }

  // Insertion so create a new entry
  red_black_node_t *z = malloc(sizeof(red_black_node_t));
  z->parent = y;
  z->key = key;
  z->value = value;
  z->left = &null_node;
  z->right = &null_node;
  z->color = COLOR_RED;

  // y is either null (this is the root of the tree,
  // or a leaf node and this new node should be linked to the left
  // or right side of y (guaranteed to be empty due to the termination
  // condition of the while above.
  if(y == &null_node) {
	tree->root = z;
  }
  else if (key < y->key) {
	y->left = z;
  }
  else {
	assert(key > y->key);
	y->right = z;
  }

  red_black_insert_fixup(tree, z);
}


void* tree_search(red_black_node_t *node, int key, bool *found)
{
  if(node == &null_node) {
	*found = false;
	return NULL;
  }

  if(key < node->key) {
	return tree_search(node->left, key, found);
  }
  else if(key > node->key) {
	return tree_search(node->right, key, found);
  }
  else {
	assert(key == node->key);
	*found = true;
	return node->value;
  }
}

void* red_black_get(red_black_t *tree, int key, bool *found)
{
  return tree_search(tree->root, key, found); 
}

