/*
 * csg -- simple static sites generator 
 * Copyright (C) 2021 Artem Baxtiarov *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>

#include "convert.h"
#include "limits.h"

char *get_relative_path(int n, char *path)
{
	char *res = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
	strcpy(res, path + n); 
	return res;
}

char *get_absolute_path(char *prefix, char *path)
{
	char *res = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
	strcpy(res, prefix);
	strcpy(res + strlen(res), path); /* ./relative/path/to/file, we don't need dot */
	return res;
}

char *get_dst(char *src, char *src_dir, int src_size, char *dst_dir)
{
	char *rpath = get_relative_path(src_size, src);
	if(rpath) {
		char *dst = get_absolute_path(dst_dir, rpath);
		if(dst) {
			return dst;
		}
		free(rpath);
	}
	exit(1);
}
