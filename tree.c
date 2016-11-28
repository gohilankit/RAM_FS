
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>       /* getuid(), getgid() */
#include <sys/types.h>

#include "tree.h"
#include "globals.h"

/*Linux has a maximum filename length of 255 characters for most filesystems
(including EXT4), and a maximum path of 4096 characters. */
#define MAX_PATH_LENGTH 4096

//global variables declared in tree.h
TreeNode* root;
char new_entry_name[256];

unsigned int fs_size;
unsigned int curr_size;

TreeNode* create_node(char* name, mode_t mode, inode_type type, int size){
  TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
  node->inode = (ram_inode*)malloc(sizeof(ram_inode));

  strcpy(node->name, name);

  node->inode->type = type;
  node->inode->mode = mode;

  node->inode->uid = getuid();
  node->inode->gid = getgid();
  node->inode->size = size;

  node->data = NULL;

  node->parent = NULL;
  node->firstChild = NULL;
  node->nextSibling = NULL;

  time_t curr_time;
  time(&curr_time);
  node->inode->atime = curr_time;
  node->inode->mtime = curr_time;
  node->inode->ctime = curr_time;

  //Update disk
  curr_size += (sizeof(TreeNode) + sizeof(ram_inode));

  return node;
}

TreeNode* create_dir_node(char* name, mode_t mode){
  int size = MAX_PATH_LENGTH;
  inode_type type = TYPE_DIR;
  mode_t type_mode = S_IFDIR | 0755;
  return create_node(name, type_mode, type, size);
}

TreeNode* create_file_node(char* name, mode_t mode){
  int size = 0;
  inode_type type = TYPE_FILE;
  mode_t type_mode = S_IFREG | mode;
  return create_node(name, type_mode, type, size);
}

TreeNode* get_node_from_path(const char* path){
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
  new_child->parent = parent;
}

void detach_child(TreeNode* parent, TreeNode* node_to_delete){
  //If node to delete is the first child
  if(parent->firstChild == node_to_delete){
    if(node_to_delete->nextSibling == NULL){
      //If node to delete is the only child, remove linking
      parent->firstChild = NULL;
    }else{
      //Make parent's first child to point to next sibling
      parent->firstChild = node_to_delete->nextSibling;
    }
  }else{
    //Iterate to reach the desired node
    TreeNode* curr = parent->firstChild;

    while(curr){
      if(curr->nextSibling == node_to_delete){
        curr->nextSibling = node_to_delete->nextSibling;
        break;
      }
      curr=curr->nextSibling;
    }
  }
}

int validatePath(const char *path)
{
        char tmpPath[MAX_PATH_LENGTH];
        strcpy(tmpPath, path);
        char *token = strtok(tmpPath, "/");

        if(token == NULL && (strcmp(path, "/") == 0))
        	return 0;
        else
        {
                int flag = 0;
                TreeNode *tmpNode  = root;
                TreeNode *temp = NULL;

                while(token != NULL) {
                	temp = tmpNode->firstChild;
                        while(temp) {
                        	if(strcmp(temp->name, token) == 0 )
                                {
                                        flag = 1;
                                        break;
                                }
                                temp = temp->nextSibling;
                        }

                        token = strtok(NULL, "/");

                        if(flag == 1) {
                        	if( token == NULL )
                                	return 0;
			}
                        else {
                                if (token)
                                	return -1;
                                else
                                	return 1;
                        }

                        tmpNode = temp;
                        flag = 0;
                }
        }
        //if it came till here then
        return -1;
}
