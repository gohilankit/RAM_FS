#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "fuse_opers.h"

int fs_size;

void create_inode(char* path){
  printf("%s \n", path);
}

void init_fs(){
  //Create inode for root directory
  char* root_path = "/";
  create_inode(root_path);
}

static struct fuse_operations hello_oper = {
	.getattr	= ramdisk_getattr
/*	.readdir	= ramdisk_readdir,
	.open		= ramdisk_open,
	.read		= ramdisk_read,
  .write = ramdisk_write,
  .create = ramdisk_create,
  .mkdir = ramdisk_mkdir,
  .rmdir = ramdisk_rmdir,
  .opendir = ramdisk_opendir,
  .unlink = ramdisk_unlink*/
};

int main(int argc, char *argv[])
{

  if(argc != 3){
    printf("Incorrect number of arguments passed\n");
    printf("Usage : ./ramdisk <path/to/dir/> <size(in MB)> \n");
  }
  //File system size(in MB) provided by the user
  int fs_size_mb = atoi(argv[2]);
  fs_size = fs_size_mb * 1024 *1024;

  init_fs();

  //Modified argv, as fuse_main takes only first two params(filesystem and mountpoint)
  char* argv_mod[2];
  argv_mod[0] = argv[0];
  argv_mod[1] = argv[1];

	return fuse_main(argc-1, argv_mod, &hello_oper, NULL);
}
