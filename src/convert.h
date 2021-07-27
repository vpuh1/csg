/*
 * csg -- simple static sites generator 
 * Copyright (C) 2021 Artem Baxtiarov
 *
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

#ifndef CONVERT_H
#define CONVERT_H

char *get_relative_path(int n, char *path);
char *get_absolute_path(char *prefix, char *path);
char *get_dst(char *src, char *src_dir, int src_size, char *dst_dir);

#endif /* CONVERT_H */
