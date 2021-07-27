#include <stdlib.h>
#include <string.h>

#include "convert.h"
#include "limits.h"

char *get_relative_path(int n, char *path) {
  char *res = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
  strcpy(res, path + n); 
  return res;
}

char *get_absolute_path(char *prefix, char *path) {
  char *res = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
  strcpy(res, prefix);
  strcpy(res + strlen(res), path); /* ./relative/path/to/file, we don't need dot */
  return res;
}

char *get_dst(char *src, char *src_dir, int src_size, char *dst_dir) {
  char *rpath = get_relative_path(src_size, src);
  if(rpath) {
    char *dst = get_absolute_path(dst_dir, rpath);
    if(dst) {
      return dst;
    }
    free(rpath);
  }
}
