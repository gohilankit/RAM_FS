
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"

#define MAX_PATH_LENGTH 512

//root is global variable declared in tree.h
TreeNode* root;

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
