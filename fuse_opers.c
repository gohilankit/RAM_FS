
#include <stdio.h>
#include "fuse_opers.h"

int ramdisk_getattr(const char *path, struct stat *stbuf){
  printf("In getattr");
  return 1;
}
