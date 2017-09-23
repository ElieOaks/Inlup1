#include <stdbool.h>
#include <stdio.h>
#include "tree.h"
#include <stdlib.h>
#include <string.h>

typedef struct node node_t;

struct tree
{
  node_t *root;
};

struct node
{
  K keys;
  T elem;
  node_t *right;
  node_t *left;
};



tree_t *tree_new()
{
  tree_t *new = calloc(1, sizeof(tree_t));
  new->root = NULL;
  return new;
}

node_t *new_node(K key, T elem)
{
  node_t *new = calloc(1, sizeof(node_t));
  new->keys = key;
  new->elem = elem;
  new->left = NULL;
  new->right = NULL;
  return new;
}

//-----------------------------------------tree-apply--------------------------------------------------

// void tree_apply(tree_t *tree, enum tree_order order, tree_action2 fun, void *data)

//------------------------------------------------size of tree -----------------------------------------
int node_count_rec(node_t *node) //NEEDS TO BE TESTED ON NON-EMPTY TREE
{
  if (node == NULL)
    {
      return 0;
    }
  else
    {
      return 1 + node_count_rec(node->left) + node_count_rec(node->right); 
    }
  
}

int tree_size(tree_t *tree)
{
  return node_count_rec(tree->root);   
}
//----------------------------------------------- tree_insert ---------------
bool tree_has_key(tree_t *tree, K key); // så att insert kan använda den funktionen.

bool tree_insert_rec(node_t *node, K key, T elem)
{
  if (strcmp((node->keys), key) > 0) //saker hamnar i bokstavs ordning mha ASCII kod. 
    {
      if (node->left == NULL)
        {
          node_t *new = new_node(key, elem);
          node->left = new;
          return true;
        }
      else
        tree_insert_rec(node->left, key, elem);
    }
  else
    {
      if (node->right == NULL)
        {
          node_t *new = new_node(key, elem);
          node->right = new;
          return true;
        }
      else
        tree_insert_rec(node->right, key, elem);
    }
  return true; //---verkar komma hit trots at dn gör return true där uppe. Samma fel på tree_get_rec Fråga TA! 
}

bool tree_insert(tree_t *tree, K key, T elem)
{
  if (tree_has_key(tree, key))
    {
      return false;
    }
  else if (tree->root == NULL)
    {
      tree->root = new_node(key, elem);
      return true;
    }
  else
    {
      return tree_insert_rec(tree->root, key, elem);
    }
}




//------------------------------------------------------------------

void cleanup(tree_t *tree)
{
  return;
}

// ------------------------Tree delete-------------------------
void node_delete_rec(node_t *node)
{
  if (node == NULL)
    {
      return;
    }
  else
    {
      node_delete_rec(node->left);
      node_delete_rec(node->right);
      free(node);
      return;
    }
}
  
void tree_delete(tree_t *tree, tree_action cleanup) // Fix me för implementation till list
{
  node_delete_rec(tree->root);
  free(tree);
  return;
}
//------------------------------ find key
bool node_has_key_rec(node_t *node, K key) //test on non empty list
{
  if (node == NULL)
    {
      return false;
    }
  else if ((node->keys) == key)
    {
      return true;
    }
  else
    {
      return ((node_has_key_rec(node->left, key)) || (node_has_key_rec(node->right, key)));
    }
}

bool tree_has_key(tree_t *tree, K key)
{
  return node_has_key_rec(tree->root, key);
}

// --------------------------------------Tree depth------
int node_depth_rec(node_t *node)
{
  if (node == NULL)
    {
      return 0;
    }
  else
    {
      int left_cunt = node_depth_rec(node->left);
      int right_cunt = node_depth_rec(node->right);
      if (left_cunt > right_cunt)
        {
          return 1 + left_cunt;
        }
      else
        {
          return 1 + right_cunt;
        }
    }
}

int tree_depth(tree_t *tree)
{
  if (tree->root == NULL)
    {
      return 0;
    }
  else
    {
      int num = node_depth_rec(tree->root);
      return num;
    }
}

// --------------------------------------
T node_get_rec(node_t *node, K key) //test on non empty list
{
  if  (strcmp((node->keys), key) == 0)
    {
      return node->elem;
    }
  else if  (strcmp((node->keys), key) > 0)
    {
      node_get_rec(node->left, key); 
    }
  else
    {
      node_get_rec(node->right, key);
    }
  return node->elem;// ska inte behövas - se felet beskrivet ovan!!!
}



T tree_get(tree_t *tree, K key) // MUST BE TESTED ON NON EMPTY TREE
{
  if (!tree_has_key(tree, key))
    {
      return NULL; //If key isn't in tree.
    }
  else if (tree->root->keys == key)
    {
      return tree->root->elem;
    }
  else
    {
      return node_get_rec(tree->root, key);
    }
}


  
//--------------------------------------------
/*
T tree_remove(tree_t *tree, K key)
{
  return;
}
*/

// --------------------tree_keys -------------------------------------------------------------------------

void tree_keys_rec(node_t *node, K *buf, int *ord) // help from stack overflow:
{
  if (node == NULL)
    {
      return;
    }
  tree_keys_rec(node->left, buf, ord);
  buf[*ord] = node->keys;
  ++(*ord);
   tree_keys_rec(node->right, buf, ord);
  return;
}

K *tree_keys(tree_t *tree)
{
  K *buf = calloc(tree_size(tree), sizeof(K));
  if (tree->root == NULL)
    {
      return 0;
    }
  else
    {
      int ord = 0;
      tree_keys_rec(tree->root, buf, &ord);
      return buf;
    }
}

int print_keys(tree_t *tree) //Denna funktion finns i lager.c
{
  K *buf = tree_keys(tree);
  int len = tree_size(tree);
  for (int i = 0; i !=  len; i++)
    {
      K current = buf[i];
      printf("%d.%s\n",i+1, current);
  }
  free(buf);
  return 0;
}

// ----------------------------------------------------------------------------------------------
/*
int main()
{
  int amount = 13;
  
  tree_t *tree = tree_new();

  tree_insert(tree, "KALAS", &amount);
  tree_insert(tree, "BAJS", &amount);
  tree_insert(tree, "AKA", &amount);
  tree_insert(tree, "ZOO", &amount);
  
  //int size = tree_size(tree);
  //K varan = (tree->root->left->left->keys);
  //int stor = tree_size(tree);
  T pajas = tree_get(tree, "U"); //Görs pajas nu till en dubbelpekare? - fråga en TA?
  int depth = tree_depth(tree);

  bool has_key = tree_has_key(tree, "BAJS");
  printf(has_key ? "Funkar tree_has_keys? Key finns? Ja" : "Funkar tree_has_keys? Key finns? Nej");
  print_keys(tree);
  printf("tree depth is %d\n", depth);
  //printf(funk1 ? "första insert seemed to work\n": "första Insert failed\n");
  //printf(funk2 ? "andra insert seemed to work\n": "andra Insert failed\n");
  //printf(funk3 ? "tredje insert seemed to work\n": "tredje Insert failed\n");

  tree_delete(tree, NULL);
 
  return 0;
}
*/
