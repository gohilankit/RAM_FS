#ifndef INIT_H
#define INIT_H

#include <sys/stat.h>

typedef enum inode_type{
  TYPE_FILE,
  TYPE_DIR
} inode_type;

//Similar to struct stat
typedef struct ram_inode{
  int id;

  char* name;
  inode_type type;
  int size;
  mode_t mode;

  //Time attributes
  time_t atime;     /* time of last access */
  time_t mtime;     /* time of last modification */
  time_t ctime;     /* time of last status change */

} ram_inode;

#endif
