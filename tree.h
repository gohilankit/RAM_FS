#ifndef TREE_H
#define TREE_H

#include <sys/stat.h>
#include "inode.h"

typedef struct TreeNode{
  char name[256];
  struct ram_inode* inode;

  struct TreeNode* parent;
  struct TreeNode* firstChild;
  struct TreeNode* nextSibling;

  char* data;
} TreeNode;

extern TreeNode* root;

extern char new_entry_name[256];

TreeNode* create_dir_node(char* name, mode_t mode);
TreeNode* create_file_node(char* name, mode_t mode);
TreeNode* get_node_from_path(const char* path);

void add_child(TreeNode* parent, TreeNode* new_child);
void detach_child(TreeNode* parent, TreeNode* node_to_delete);

int validatePath(const char *path);

#endif
