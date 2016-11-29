
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "fuse_opers.h"
#include "tree.h"
#include "globals.h"

unsigned int fs_size;
unsigned int curr_size;

//global variables declared in tree.h
TreeNode* root;
char new_entry_name[256];

int ramdisk_getattr(const char* path, struct stat* stbuf){
  int res = 0;

  if(validatePath(path) == 0){

    TreeNode* node = get_node_from_path(path);

    stbuf->st_atime = node->inode->atime;
    stbuf->st_mtime = node->inode->mtime;
    stbuf->st_ctime = node->inode->ctime;

    //stbuf->st_nlink
    stbuf->st_mode  = node->inode->mode;

    stbuf->st_uid = node->inode->uid;
    stbuf->st_gid = node->inode->gid;

    stbuf->st_size = node->inode->size;
    res = 0;
  }else{
    res = -ENOENT;
  }
  return res;
}

int ramdisk_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
                    off_t offset, struct fuse_file_info* fi){

    if(validatePath(path) == -1)
      return -ENOENT;
    //else
    TreeNode* dir = get_node_from_path(path);
    TreeNode* curr = dir->firstChild;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    while(curr){
      filler(buf, curr->name, NULL, 0);
      curr = curr->nextSibling;
    }

    time_t curr_time;
    time(&curr_time);
    dir->inode->atime = curr_time;

    return 0;
}

int ramdisk_open(const char* path, struct fuse_file_info* fi){
  if(validatePath(path) != 0){
    return -ENOENT;
  }

  TreeNode* node = get_node_from_path(path);

  if(node->inode->type == TYPE_DIR){
    return -EISDIR;
  }

  return 0;
}

/*read
Read 'size' bytes into buf, beginning offset. Returns
a. number of bytes transferred,
b. 0 if offset was at or beyond the end of the file.
*/
int ramdisk_read(const char* path, char* buf, size_t size, off_t offset,
  struct fuse_file_info* fi){

  if(validatePath(path) == -1){
    return -ENOENT;
  }
//else
  TreeNode* node = get_node_from_path(path);
  if(node->inode->type == TYPE_DIR){
    //Can't perform read as it's a directory
    return -EISDIR;
  }

  size_t file_length = node->inode->size;
  if(offset > file_length){
    size = 0;
  }else{
    if(offset+size > file_length){
      size = file_length - offset;
    }
    memcpy(buf, node->data + offset, size);
    time_t curr_time;
    time(&curr_time);
    node->inode->atime = curr_time;
  }

  return size;
}

int ramdisk_write(const char* path, const char *buf, size_t size, off_t offset,
  struct fuse_file_info* fi){

  if(validatePath(path) == -1){
    return -ENOENT;
  }
//else
  if(curr_size + size > fs_size){
    //No space left on filesystem
    return -ENOSPC;
  }

  TreeNode* node = get_node_from_path(path);
  if(node->inode->type == TYPE_DIR){
    //Can't perform read as it's a directory
    return -EISDIR;
  }

  size_t file_length = node->inode->size;

  if(size > 0){
    if(node->data == NULL){
      offset = 0;
      node->data = (char*) malloc(sizeof(char)*size);
    }else{  /*Realloc new memory location for more data*/
      //If offset to write is more than current file length, adjust the offset
      if(offset > file_length)
        offset = file_length;

      char* new_block = (char*) realloc(node->data, sizeof(char)*(offset + size));
      if(new_block == NULL){
        return -ENOSPC;
      }else{
        node->data = new_block;
      }
    }

    memcpy(node->data + offset, buf, size);
    curr_size += (offset + size - node->inode->size); /*Update disk size */
    node->inode->size = offset+size;  /*Update inode size*/

    //Update create & modified time
    time_t curr_time;
    time(&curr_time);
    node->inode->ctime = curr_time;
    node->inode->mtime = curr_time;
  }
  return size;
}

int ramdisk_create(const char* path, mode_t mode, struct fuse_file_info* fi){
  if(path == NULL){
    return -ENOENT;
  }

  //Check if assigning a new node doesn't exceed the max file size
  if(curr_size + sizeof(TreeNode) + sizeof(ram_inode) > fs_size){
    return -ENOSPC;
  }

  //Get parent node
  TreeNode* parent = get_node_from_path(path);

  TreeNode* new_entry = create_file_node(new_entry_name, mode);

  add_child(parent, new_entry);
  /*Create ends. File size is updated in create_node*/
  return 0;
}

/*Create directory with @path as name and @mode as permissions */
int ramdisk_mkdir(const char* path, mode_t mode){
  if(path == NULL){
    return -ENOENT;
  }

  //Check if assigning a new node doesn't exceed the max file size
  if(curr_size + sizeof(TreeNode) + sizeof(ram_inode) > fs_size){
    return -ENOSPC;
  }

  //Get parent node
  TreeNode* parent = get_node_from_path(path);

  TreeNode* new_entry = create_dir_node(new_entry_name, mode);

  add_child(parent, new_entry);
  /* Mkdir ends. Filesystem size is updated in create_node. */
  return 0;
}

int ramdisk_rmdir(const char* path){
  if(validatePath(path) != 0){
    return -ENOENT;
  }
//else
  TreeNode* node_to_delete = get_node_from_path(path);

  if(node_to_delete->firstChild){
    //Directory is not empty, don't delete. Throw error
    return -ENOTEMPTY;
  }else{
    TreeNode* parent = node_to_delete->parent;
    detach_child(parent, node_to_delete);

    free(node_to_delete->inode);
    free(node_to_delete);

    //Update current file system size
    curr_size -= (sizeof(ram_inode) + sizeof(TreeNode));

    return 0;
  }
}

int ramdisk_opendir(const char* path, struct fuse_file_info* fi){
  if(validatePath(path) != 0){
    return -ENOENT;
  }

  TreeNode* node = get_node_from_path(path);

  if(node->inode->type != TYPE_DIR){
    return -ENOTDIR;
  }

  return 0;
}

int ramdisk_unlink(const char* path){
  if(validatePath(path) != 0){
    return -ENOENT;
  }
//else
  TreeNode* node_to_delete = get_node_from_path(path);
  if(node_to_delete->inode->type == TYPE_DIR){
    //Can't unlink as the node is a directory
    return -EISDIR;
  }

  TreeNode* parent = node_to_delete->parent;
  detach_child(parent, node_to_delete);

  if(node_to_delete->inode->size != 0){
    free(node_to_delete->data);
    curr_size -= node_to_delete->inode->size;
  }

  free(node_to_delete->inode);
  free(node_to_delete);

  curr_size -= (sizeof(TreeNode) + sizeof(ram_inode));

  return 0;
}

int ramdisk_utime(const char* path, struct utimbuf* ubuf){
  return 0;
}

int ramdisk_rename(const char *from, const char *to){
  if(validatePath(from) != 0){
    return -ENOENT;
  }
//else
/*  int is_to_valid = validatePath(to);

  TreeNode* from_node = get_node_from_path(from);
  TreeNode* to_node = get_node_from_path(to);

  if(is_to_valid){  //'to' path already exists

}*/

  return 0;
}
