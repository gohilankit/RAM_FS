#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "fuse_opers.h"
#include "tree.h"

unsigned int fs_size;
unsigned int curr_size;

//global variables declared in tree.h
TreeNode* root;
char new_entry_name[256];

#define MAXLEN 4096

void create_root_node(){
  //Create node for root directory
  root = (TreeNode*)malloc(sizeof(TreeNode));
  root->inode = (ram_inode*)malloc(sizeof(ram_inode));

  //root->name = (char*)malloc(sizeof(char));
  strcpy(root->name, "/");

  root->inode->type = TYPE_DIR;

  root->data = NULL;

  root->inode->mode = S_IFDIR | 0755;
  root->inode->uid = 0;
  root->inode->gid = 0;

  root->parent = NULL;
  root->firstChild = NULL;
  root->nextSibling = NULL;

  time_t curr_time;
  time(&curr_time);
  root->inode->atime = curr_time;
  root->inode->mtime = curr_time;
  root->inode->ctime = curr_time;

  //Update disk
  curr_size += (sizeof(TreeNode) + sizeof(ram_inode));

  //test_func();
  //ramdisk_mkdir_test();
}

void init_fs(){
  create_root_node();
}

static struct fuse_operations hello_oper = {
	.getattr	= ramdisk_getattr,
  .mkdir = ramdisk_mkdir,
	.readdir	= ramdisk_readdir,
	.open		= ramdisk_open,
	.read		= ramdisk_read,
  .write = ramdisk_write,
  .create = ramdisk_create,
  .rmdir = ramdisk_rmdir,
  .opendir = ramdisk_opendir,
  .unlink = ramdisk_unlink
};


int main(int argc, char *argv[])
{

  if(argc < 3){
    printf("Incorrect number of arguments passed\n");
    printf("Usage : ./ramdisk <path/to/dir/> <size(in MB)> \n");
  }
  //File system size(in MB) provided by the user
  int fs_size_mb = atoi(argv[argc-1]);
  fs_size = fs_size_mb * 1024 * 1024;

  init_fs();

  //Modified argv, as fuse_main takes only first two params(filesystem and mountpoint)
  char* argv_mod[3];
  argv_mod[0] = argv[0];
  argv_mod[1] = argv[1];
  argv_mod[2] = argv[2];

	return fuse_main(argc-1, argv_mod, &hello_oper, NULL);
}
