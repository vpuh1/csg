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
#ifdef __APPLE__
#include <sys/syslimits.h>
#elif __linux__
#include <linux/limits.h>
#elif __FreeBSD__
#include <sys/syslimits.h>
#endif

#define MAX_TITLE_LEN NAME_MAX
#define MAX_DATE_LEN NAME_MAX
#define MAX_NAME_LEN NAME_MAX
#define MAX_SRC_LEN PATH_MAX
#define MAX_DST_LEN PATH_MAX

typedef struct {
  char title[MAX_TITLE_LEN]; /* Article title */
  char date[MAX_DATE_LEN];  /* Article date */
  char name[MAX_NAME_LEN]; /* file name */
  char src[MAX_SRC_LEN]; /* Source path */
  char dst[MAX_DST_LEN]; /* Destination path */
} art; /* Article */
