#ifndef CONVERT_H
#define COVERT_H

char *get_relative_path(int n, char *path);
char *get_absolute_path(char *prefix, char *path);
char *get_dst(char *src, char *src_dir, int src_size, char *dst_dir);

#endif /* CONVERT_H */
