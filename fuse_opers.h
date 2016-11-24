#ifndef FUSE_OPERS_H
#define FUSE_OPERS_H

#include <sys/stat.h>


extern int fs_size;

int ramdisk_getattr(const char*, struct stat *);

#endif
