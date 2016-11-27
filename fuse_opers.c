
#include <stdio.h>
#include "fuse_opers.h"
#include "tree.h"

//root is global variable declared in tree.h
TreeNode* root;

int test_func(){
  printf("%s \n", root->name);
  return 1;
}

int ramdisk_getattr(const char* path, struct stat* stbuf){
  printf("In getattr \n");
  //int retVal = 0;

  //Todo
  //Validate path

  //Node* node = get_node_from_path()
  return 1;
}


int ramdisk_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
                    off_t offset, struct fuse_file_info* fi){

  return 1;
}

int ramdisk_open(const char* path, struct fuse_file_info* fi){
  return 1;
}

int ramdisk_read(const char* path, char* buf, size_t size, off_t offset,
  struct fuse_file_info* fi){

  return 1;
}

int ramdisk_write(const char* path, const char *buf, size_t size, off_t offset,
  struct fuse_file_info* fi){

  return 1;
}

int ramdisk_create(const char* path, mode_t mode, struct fuse_file_info* fi){

  return 1;
}

int ramdisk_mkdir(const char* path, mode_t mode){

  return 1;
}

int ramdisk_rmdir(const char* path){

  return 1;
}

int ramdisk_opendir(const char* path, struct fuse_file_info* fi){

  return 1;
}

int ramdisk_unlink(const char* path){

  return 1;
}
