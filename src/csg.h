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

#ifndef CSG_H
#define CSG_H

#include "limits.h"

struct art {                 		/* Article */
	char title[CSG_NAME_MAX];  	/* Article title */
	char date[CSG_NAME_MAX];    	/* Article date */
	char name[CSG_NAME_MAX];    	/* file name */
	char src[CSG_PATH_MAX];      	/* Source path */
	char dst[CSG_PATH_MAX]; 	/* Destination path */
};

#endif /* CSG_H */
