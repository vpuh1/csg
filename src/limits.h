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

#ifndef LIMITS_H
#define LIMITS_H

#ifdef __APPLE__
#include <sys/syslimits.h>
#elif __linux__
#include <linux/limits.h>
#elif __FreeBSD__
#include <sys/syslimits.h>
#endif 

#define CSG_PATH_MAX PATH_MAX
#define CSG_NAME_MAX NAME_MAX

#ifdef __linux__
#define CSG_ARG_MAX sysconf(_SC_ARG_MAX)
#else
#define CSG_ARG_MAX ARG_MAX
#endif

#endif /* LIMITS_H */
