#ifndef TREE_H
#define TREE_H

#include "inode.h"

typedef struct TreeNode{
  char* name;
  struct ram_inode* inode;

  struct TreeNode* parent;
  struct TreeNode* firstChild;
  struct TreeNode* nextSibling;

  char* data;
} TreeNode;

#endif
