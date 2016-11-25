
#include <stdio.h>
#include "fuse_opers.h"

int test_func(){
  printf("%d \n", fs_size);
  return 1;
}

int ramdisk_getattr(const char* path, struct stat* stbuf){
  printf("In getattr \n");
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
