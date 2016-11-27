all:
	gcc -Wall fuse_opers.h inode.h fuse_opers.c tree.h tree.c main.c `pkg-config fuse --cflags --libs` -o ramdisk
