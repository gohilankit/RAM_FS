all:
	gcc -Wall fuse_opers.h inode.h fuse_opers.c main.c `pkg-config fuse --cflags --libs` -o ramdisk
