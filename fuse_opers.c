
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "fuse_opers.h"
#include "tree.h"
#include "globals.h"

unsigned int fs_size;
unsigned int curr_size;

//global variables declared in tree.h
TreeNode* root;
char new_entry_name[256];

int test_func(){
  printf("%s \n", root->name);
  return 1;
}

int ramdisk_getattr(const char* path, struct stat* stbuf){
  printf("In getattr for path : %s \n", path);
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

    //stbuf->st_size = 2028;
    stbuf->st_size = node->inode->size;
    res = 0;
    printf("getattr ends normally for path : %s \n", path);
  }else{
    res = -ENOENT;
  }
  return res;
}

int ramdisk_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
                    off_t offset, struct fuse_file_info* fi){

    printf("In readdir for path : %s \n", path);

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

  printf("In open \n");
  return 0;
}

int ramdisk_read(const char* path, char* buf, size_t size, off_t offset,
  struct fuse_file_info* fi){

  printf("In read \n");
  return 0;
}

int ramdisk_write(const char* path, const char *buf, size_t size, off_t offset,
  struct fuse_file_info* fi){

  printf("In write \n");
  return 0;
}

int ramdisk_create(const char* path, mode_t mode, struct fuse_file_info* fi){
  printf("In create \n");
  return 0;
}

/*Create directory with @path as name and @mode as permissions */
int ramdisk_mkdir(const char* path, mode_t mode){
  printf("In mkdir \n");
  if(path == NULL){
    return -ENOENT;
  }

  //Check if assigning a new node doesn't exceed the max file size
  if(curr_size + sizeof(TreeNode) + sizeof(ram_inode) > fs_size){
      printf("FS Size = %d \n", fs_size);
      printf("Curr Size = %ld \n", (curr_size + sizeof(TreeNode) + sizeof(ram_inode)) );
      return -ENOSPC;
  }

  //Get parent node
  TreeNode* parent = get_node_from_path(path);

  TreeNode* new_entry = create_dir_node(new_entry_name, mode);

  add_child(parent, new_entry);
  printf("Mkdir ends\n");
  return 0;
}

/*Create directory with @path as name and @mode as permissions */
int ramdisk_mkdir_test(){

  //Check if assigning a new node doesn't exceed the max file size
  if(curr_size + sizeof(TreeNode) + sizeof(ram_inode) > fs_size){
      return -ENOSPC;
  }

  const char* path = "/a";
  mode_t mode = S_IFDIR | 0755;
  //Get parent node
  TreeNode* parent = get_node_from_path(path);
  printf("Parent = %s \n", parent->name);

  TreeNode* new_entry = create_dir_node(new_entry_name, mode);
  printf("New entry = %s \n", new_entry->name);

  add_child(parent, new_entry);

  printf("New entry's child = %s \n", new_entry->parent->name);
  printf("Parent's child = %s \n", parent->firstChild->name);

  return 0;
}

int ramdisk_rmdir(const char* path){
  printf("In rmdir \n");
  return 0;
}

int ramdisk_opendir(const char* path, struct fuse_file_info* fi){
  printf("In opendir \n");
  return 0;
}

int ramdisk_unlink(const char* path){
  printf("In unlink \n");
  return 0;
}
