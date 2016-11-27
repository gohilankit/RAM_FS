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

extern TreeNode* root;

extern char new_entry_name[256];

TreeNode* get_node_from_path(char* path);

void add_child(TreeNode* parent, TreeNode* new_child);

#endif
