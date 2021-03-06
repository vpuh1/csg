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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wordexp.h>

#include "config.h"
#include "limits.h"

void init_config_values(struct config *conf, struct config_values *conf_val)
{
	strcpy(conf_val[0].name, "highlight_theme");
	conf_val[0].var = conf->highlight_theme;
	strcpy(conf_val[1].name, "main_page_title");
	conf_val[1].var = conf->mp_title;
	strcpy(conf_val[2].name, "main_page_css");
	conf_val[2].var = conf->mp_css;
	strcpy(conf_val[3].name, "main_page_header");
	conf_val[3].var = conf->mp_header;
	strcpy(conf_val[4].name, "main_page_footer");
	conf_val[4].var = conf->mp_footer;
	strcpy(conf_val[5].name, "main_page_name");
	conf_val[5].var = conf->mp_name;
	strcpy(conf_val[6].name, "article_css");
	conf_val[6].var = conf->art_css;
	strcpy(conf_val[7].name, "article_template");
	conf_val[7].var = conf->art_template;
	strcpy(conf_val[8].name, "article_header");
	conf_val[8].var = conf->art_header;
	strcpy(conf_val[9].name, "article_footer");
	conf_val[9].var = conf->art_footer;
}

char *get_user()
{
	char *username = (char *) malloc(sizeof(char) * CSG_NAME_MAX);
	getlogin_r(username, CSG_NAME_MAX);
	if(username[strlen(username) - 1] != '\0')
		username[strlen(username)] = '\0';
	return username;
}

void replace_tilde(char *home, char *path)
{
	char *no_tilde = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
	if(path[0] == '~') {
		strcpy(no_tilde, path + 1);
		strcpy(path, home);
		strcpy(path + strlen(path), no_tilde);
	}
}

char *get_config()
{
	char *username = get_user();
	char *home = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
#ifdef __APPLE__
	strcpy(home, "/Users/");
#else
	strcpy(home, "/home/");
#endif
	strcpy(home + strlen(home), username);
	char *config_path = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
	strcpy(config_path, home);
	strcpy(config_path + strlen(config_path), "/.csg/csgrc");
	return config_path;
}

char *read_config(FILE *config)
{
	char *buf = (char *) malloc(sizeof(char) *(CSG_PATH_MAX * 7 + CSG_NAME_MAX * 13));
	int i = 0;
	while((buf[i] = fgetc(config)) != EOF)
		i++;
	buf[i - 1] = '\0';
	return buf;
}

void parse_config(char *buf, int size, struct config conf, 
		struct config_values *conf_val)
{
	for(int i = 0; i < size; i++) {
		if(buf[i] == '#' && (i == 0 || buf[i - 1] == '\n')) {
			while(buf[i] != '\n')
				i++;
			if(i + 1 < size)
				i++;
			else
				return;
		}
		char var[CSG_NAME_MAX], val[CSG_PATH_MAX];
		int k = 0;
		for(; !(buf[i] == ' ' || buf[i] == '='); i++, k++)
			var[k] = buf[i];
		var[k] = '\0';
		for(; buf[i] != '"'; i++);
		if(i + 1 < size)
			i++;
		else
			return;
		int j = 0;
		for(; buf[i] != '"'; i++, j++) {
			val[j] = buf[i];
		}
		val[j] = '\0';
		for(int j = 0; j < NUM_CONFIG_VALUES; j++) {
			if(strcmp(var, conf_val[j].name) == 0) {
				strcpy(conf_val[j].var, val);
				break;
			}
		}
		for(; buf[i] != '\n'; i++);
	}
}

void set_default_config(struct config conf) {
	strcpy(conf.highlight_theme, "breezedark");
	strcpy(conf.mp_title, "csg");
	strcpy(conf.mp_css, "/etc/csg/css/index.css");
	strcpy(conf.mp_header, "/etc/csg/html/mpheader.html");
	strcpy(conf.mp_footer, "/etc/csg/html/mpfooter.html");
	strcpy(conf.mp_name, "index.html");
	strcpy(conf.art_css, "/etc/csg/css/pandoc.css");
	strcpy(conf.art_template, "/etc/csg/html/template.html");
	strcpy(conf.art_header, "/etc/csg/html/header.html");
	strcpy(conf.art_footer, "/etc/csg/html/footer.html");
}


void replace_tilde_all(struct config *conf) /* use /home/username/ or /Users/username instead of ~/ */
{ 
	char *username = get_user();
	char *home = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
#ifdef __APPLE__
	strcpy(home, "/Users/");
#else
	strcpy(home, "/home/");
#endif
	strcpy(home + strlen(home), username);
	replace_tilde(home, conf->highlight_theme);
	replace_tilde(home, conf->mp_css);
	replace_tilde(home, conf->mp_header);
	replace_tilde(home, conf->mp_footer);
	replace_tilde(home, conf->art_css);
	replace_tilde(home, conf->art_template);
	replace_tilde(home, conf->art_header);
	replace_tilde(home, conf->art_footer);
}

void open_config(struct config *conf, struct config_values *conf_val)
{
	char *config_path = get_config();
	if(config_path) {
		FILE *config = fopen(config_path, "r");
		if(config) {
			char *buf = read_config(config);
			if(buf) {
				parse_config(buf, strlen(buf), *conf, conf_val);
				replace_tilde_all(conf);
				free(buf);
			}
			fclose(config);
			return;
		}
	} 

	fprintf(stderr, "csg: no %s specified, using /etc/csg/csgrc instead\n", config_path);
	FILE *config = fopen("/etc/csg/csgrc", "r");
	if(config) {
		char *buf = read_config(config);
		if(buf) {
			parse_config(buf, strlen(buf), *conf, conf_val);
			replace_tilde_all(conf);
			free(buf);
		}
		fclose(config);
	}
	else {
		set_default_config(*conf);
		exit(1);
	}
	free(config_path);
}
