#include "tree.h"

#define MAX_PATH_LENGTH 512

TreeNode* get_node_from_path(TreeNode* root, char* path){
  char path_copy[MAX_PATH_LENGTH];

  strcpy(path_copy, path);

  char* token = strtok(path_copy, "/");

  //If path is root directory("/"), return root node
  if(strcmp(path, "/") == 0 && token == NULL) {
    return root;
  }
//else
  int found = 0;

  Node* start_node = root;
  Node* curr = NULL;

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
        return temp;
      }
    }

    start_node = curr;
    found = 0;
  }
  return NULL;
}
