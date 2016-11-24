#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>



static struct fuse_operations hello_oper = {
	.getattr	= ramdisk_getattr,
	.readdir	= ramdisk_readdir,
	.open		= ramdisk_open,
	.read		= ramdisk_read,
  .write = ramdisk_write,
  .create = ramdisk_create,
  .mkdir = ramdisk_mkdir,
  .rmdir = ramdisk_rmdir,
  .opendir = ramdisk_opendir,
  .unlink = ramdisk_unlink
};

int main(int argc, char *argv[])
{
  //File system size in MB provided by the user
  int fs_size = atoi(argv[2]);

  //Modified argv, as fuse_main takes only first two params(filesystem and mountpoint)
  char* argv_mod[2];
  argv_mod[0] = argv[0];
  argv_mod[1] = argv[1];
  
	return fuse_main(argc, argv_mod, &hello_oper, NULL);
}
