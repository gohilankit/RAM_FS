#ifndef INIT_H
#define INIT_H

#include <sys/stat.h>

typedef enum inode_type{
  FILE
  DIR
} inode_type;

struct ram_inode{
  int id;

  char* name;
  inode_type type;
  int size;
  mode_t mode;

  char* data;

}

#endif
