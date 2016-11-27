
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"

/*Linux has a maximum filename length of 255 characters for most filesystems
(including EXT4), and a maximum path of 4096 characters. */
#define MAX_PATH_LENGTH 512

//global variables declared in tree.h
TreeNode* root;
char new_entry_name[256];

TreeNode* get_node_from_path(char* path){
  char path_copy[MAX_PATH_LENGTH];

  strcpy(path_copy, path);

  char* token = strtok(path_copy, "/");

  //If path is root directory("/"), return root node
  if(strcmp(path, "/") == 0 && token == NULL) {
    return root;
  }
//else
  int found = 0;

  TreeNode* start_node = root;
  TreeNode* curr = NULL;

  while(token){
    curr = start_node->firstChild;
    while(curr){
      if(strcmp(token, curr->name) == 0 ){
        found = 1;
        break;
      }
      curr = curr->nextSibling;
    }

    if(found) {
      token = strtok(NULL, "/");
      if(token == NULL ){
        return curr;
      }
    }else{
      /* If entry not found(not in children of start_node)
      1. Copy the token in a global variable for new entry
      2. Return the parent(start_node)
      This is required for mkdir where last token is name of the directory
      and path before that is the parent directory*/
      strcpy(new_entry_name, token);
      return start_node;
    }

    start_node = curr;
    found = 0;
  }
  return NULL;
}

void add_child(TreeNode* parent, TreeNode* new_child){
  if(parent == NULL || new_child == NULL)
    return;

  if(parent->firstChild != NULL){
    TreeNode* curr = parent->firstChild;
    while(curr->nextSibling != NULL){
      curr = curr->nextSibling;
    }
    curr->nextSibling = new_child;
  }else{
    parent->firstChild = new_child;
  }
}
