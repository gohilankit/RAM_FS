#ifndef FUSE_OPERS_H
#define FUSE_OPERS_H

#include <sys/stat.h>
#include <fuse.h>

int ramdisk_getattr(const char*, struct stat *);

int ramdisk_readdir(const char*, void*, fuse_fill_dir_t, off_t, struct fuse_file_info* fi);

int ramdisk_open(const char* path, struct fuse_file_info* fi);

int ramdisk_read(const char* path, char *buf, size_t size, off_t offset, struct fuse_file_info* fi);

int ramdisk_write(const char* path, const char *buf, size_t size, off_t offset, struct fuse_file_info* fi);

int ramdisk_create(const char *, mode_t, struct fuse_file_info *);

int ramdisk_mkdir(const char *, mode_t);

int ramdisk_rmdir(const char *);

int ramdisk_opendir(const char* path, struct fuse_file_info* fi);

int ramdisk_unlink(const char *);

int ramdisk_utime(const char* path, struct utimbuf* ubuf);

#endif
