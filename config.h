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

#ifndef CONFIG_H
#define CONFIG_H

#ifdef __APPLE__
#include <sys/syslimits.h>
#elif __linux__
#include <linux/limits.h>
#elif __FreeBSD__
#include <sys/syslimits.h>
#endif

#include <stdio.h>

#define NUM_CONFIG_VALUES 10

struct config {
  char highlight_theme[NAME_MAX];  /* code highlight theme, see comment below
                                      (Highlight themes) for more info */
  char mp_title[NAME_MAX];         /* main page title */
  char mp_css[PATH_MAX];           /* main page css */
  char mp_header[PATH_MAX];        /* main page header */
  char mp_footer[PATH_MAX];        /* main page footer */
  char mp_name[NAME_MAX];          /* main page name */

  char art_css[PATH_MAX];      /* css for the article*/
  char art_template[PATH_MAX]; /* template for the article */
  char art_header[PATH_MAX];   /* header for the article */
  char art_footer[PATH_MAX];   /* footer for the article */
};
/* 
 * Highlight Themes.
 *
 * Available highlight themes: pygments, kate, monochrome, breezedark,
 * espresso, zenburn, haddock, tango.
 *
 * You can provide your own custom theme:
 *   pandoc --print-highlight-style pygments > my.theme
 * Then edit my.theme.
 */
struct config_values { /* config values to parse */
  char name[NAME_MAX]; /* name in config file */
  char *var; /* corresponding variable */
};

struct config conf; /* csg config */
struct config_values conf_val[NUM_CONFIG_VALUES]; /* csg config values */

/* functions declaration */
void init_config_values();
char *get_user();
char *get_config();
char *read_config(FILE *config);
void parse_config(char *buf, int size);
void set_default_config();
void open_config();

#endif /* CONFIG_H */
