#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wordexp.h>

#include "config.h"

void init_config_values() {
  strcpy(conf_val[0].name, "highlight_theme");
  conf_val[0].var = conf.highlight_theme;
  strcpy(conf_val[1].name, "main_page_title");
  conf_val[1].var = conf.mp_title;
  strcpy(conf_val[2].name, "main_page_css");
  conf_val[2].var = conf.mp_css;
  strcpy(conf_val[3].name, "main_page_header");
  conf_val[3].var = conf.mp_header;
  strcpy(conf_val[4].name, "main_page_footer");
  conf_val[4].var = conf.mp_footer;
  strcpy(conf_val[5].name, "main_page_name");
  conf_val[5].var = conf.mp_name;
  strcpy(conf_val[6].name, "article_css");
  conf_val[6].var = conf.art_css;
  strcpy(conf_val[7].name, "article_template");
  conf_val[7].var = conf.art_template;
  strcpy(conf_val[8].name, "article_header");
  conf_val[8].var = conf.art_header;
  strcpy(conf_val[9].name, "article_footer");
  conf_val[9].var = conf.art_footer;
}

char *get_user() {
  char *username = (char *) malloc(sizeof(char) * NAME_MAX);
  getlogin_r(username, NAME_MAX);
  if(username[strlen(username) - 1] != '\0')
    username[strlen(username)] = '\0';
  return username;
}

char *get_config() {
  char *username = get_user();
  if(username) {
    char *config_path = (char *) malloc(sizeof(char) * PATH_MAX);
    strcpy(config_path, "/home/");
    strcpy(config_path + strlen(config_path), username);
    strcpy(config_path + strlen(config_path), "/.csg/csgrc");

    free(username);
    return config_path;
  } else {
    fprintf(stderr, "csg: cannot get username\n");
    exit(1);
  }
}

char *read_config(FILE *config) {
  char *buf = (char *) malloc(sizeof(char) *(PATH_MAX * 7 + NAME_MAX * 13));
  int i = 0;
  while((buf[i] = fgetc(config)) != EOF)
    i++;
  buf[i - 1] = '\0';
  return buf;
}

void parse_config(char *buf, int size) {
  for(int i = 0; i < size; i++) {
    if(buf[i] == '#' && (i == 0 || buf[i - 1] == '\n')) {
      while(buf[i] != '\n')
        i++;
      if(i + 1 < size)
        i++;
      else
        return;
    }
    char var[NAME_MAX], val[PATH_MAX];
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
    for(int j = 0; j < 10; j++) {
      if(strcmp(var, conf_val[j].name) == 0) {
        strcpy(conf_val[j].var, val);
        break;
      }
    }
    for(; buf[i] != '\n'; i++);
  }
}

void set_default_config() {
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

void replace_tilde() { /* use /home/username/ instead of ~/ */
  wordexp_t no_tilde;
  conf.highlight_theme[strlen(conf.highlight_theme)] = '\0';
  wordexp(conf.highlight_theme, &no_tilde, 0);
  strcpy(conf.highlight_theme, no_tilde.we_wordv[0]);
  wordfree(&no_tilde);
  conf.mp_css[strlen(conf.mp_css)] = '\0';
  wordexp(conf.mp_css, &no_tilde, 0);
  strcpy(conf.mp_css, no_tilde.we_wordv[0]);
  wordfree(&no_tilde);
  conf.mp_header[strlen(conf.mp_header)] = '\0';
  wordexp(conf.mp_header, &no_tilde, 0);
  strcpy(conf.mp_header, no_tilde.we_wordv[0]);
  wordfree(&no_tilde);
  conf.mp_footer[strlen(conf.mp_footer)] = '\0';
  wordexp(conf.mp_footer, &no_tilde, 0);
  strcpy(conf.mp_footer, no_tilde.we_wordv[0]);
  wordfree(&no_tilde);
  conf.art_css[strlen(conf.art_css)] = '\0';
  wordexp(conf.art_css, &no_tilde, 0);
  strcpy(conf.art_css, no_tilde.we_wordv[0]);
  wordfree(&no_tilde);
  conf.art_template[strlen(conf.art_template)] = '\0';
  wordexp(conf.art_template, &no_tilde, 0);
  strcpy(conf.art_template, no_tilde.we_wordv[0]);
  wordfree(&no_tilde);
  conf.art_header[strlen(conf.art_header)] = '\0';
  wordexp(conf.art_header, &no_tilde, 0);
  strcpy(conf.art_header, no_tilde.we_wordv[0]);
  wordfree(&no_tilde);
  conf.art_footer[strlen(conf.art_footer)] = '\0';
  wordexp(conf.art_footer, &no_tilde, 0);
  strcpy(conf.art_footer, no_tilde.we_wordv[0]);
  wordfree(&no_tilde);
}

void open_config() {
  char *config_path = get_config();
  if(config_path) {
    FILE *config = fopen(config_path, "r");
    if(config) {
      char *buf = read_config(config);
      if(buf) {
        parse_config(buf, strlen(buf));
        replace_tilde();
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
      parse_config(buf, strlen(buf));
      replace_tilde();
      free(buf);
    }
    fclose(config);
  }
  else {
    set_default_config();
    exit(1);
  }
  free(config_path);
}
