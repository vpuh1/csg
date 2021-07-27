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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/wait.h>

#include "csg.h"
#include "config.h"
#include "limits.h"
#include "convert.h"

char *list_md_t = "find %s | grep '.md$'";             /* list all .md files */
char *make_dir_t = "mkdir -p %s";                      /* make dst dir */
char *get_title_t = "cat %s | grep title | cut -c 8-"; /* get title */
char *get_date_t = "cat %s | grep date | cut -c 7-";   /* get date */
char *get_dir_t = "find %s -mindepth 1 -maxdepth 1 -type d";
/* get subdirectories of src directory */
char *rm_md_t = "find %s -type f -name '*.md' -exec rm {} +";
/* remove .md files from dst directory */

struct config conf;
struct config_values conf_val[NUM_CONFIG_VALUES];

/* execute and get output */
char *exec_output(char *cmd) {
  char *output = (char *) malloc(sizeof(char) * CSG_ARG_MAX);
  int fd[2];

  if(pipe(fd) == -1) {
    fprintf(stderr, "csg: cannot create pipe\n");
    exit(1);
  }

  pid_t p = fork();

  if(p == -1) {
    fprintf(stderr, "csg: cannot fork process: %d\n", p);
    exit(1);
  }

  if(p == 0) {
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    execlp("sh", "sh", "-c", cmd, NULL);
    fprintf(stderr, "csg: execlp error\n");
    exit(EXIT_FAILURE);
  } else {
    close(fd[1]);
    int nbytes = read(fd[0], output, CSG_ARG_MAX);
    output[nbytes] = '\0';
    wait(NULL);
  }

  return output;
}

/* get number of articles */
int get_nart(char *buff, int size) {
  int i;
  int cnt = 0;

  for(i = 0; i < size; i++) {
    if(buff[i] == '\n' || buff[i] == '\0')
      cnt++;
  }

  return cnt;
}

void parse_articles(struct art *article, int size, char *buff) {
  int k = 0;
  int beg = 0, end = 0;

  for(end = 0; end < size; end++) {
    if(buff[end] == '\n') {
      memcpy(article[k].src, &buff[beg], end - beg);
      article[k].src[end - beg] = '\0';
      k++;
      beg = end + 1;
    }
  }
}

/* get article's title and date */
void get_info(int nart, struct art *article) {
  for(int i = 0; i < nart; i++) {
    char *get_title = (char *) malloc(sizeof(char) * CSG_NAME_MAX);
    char *get_date = (char *) malloc(sizeof(char) * CSG_NAME_MAX);

    sprintf(get_title, get_title_t, article[i].src);
    sprintf(get_date, get_date_t, article[i].src);

    char *title = exec_output(get_title);
    char *date = exec_output(get_date);

    free(get_title);
    free(get_date);

    strcpy(article[i].title, title);
    strcpy(article[i].date, date);

    free(title);
    free(date);
  }
}

/* generate dst path */
void gen_dst(int nart, struct art *article, char *src_dir, int src_size, char *dst_dir) { 
  char *name = (char *) malloc(sizeof(char) * CSG_NAME_MAX);
  int i, j;

  for(i = 0; i < nart; i++) {
    strcpy(article[i].dst, get_dst(article[i].src, src_dir, src_size, dst_dir));

    int k = strlen(article[i].dst);
    article[i].dst[k - 2] = 'h';
    article[i].dst[k - 1] = 't';
    article[i].dst[k] = 'm';
    article[i].dst[k + 1] = 'l';
    article[i].dst[k + 2] = '\0';

    for(j = strlen(article[i].src) - 1; j >= 0; j--) {
      if(article[i].src[j] == '/') {
        memcpy(name, &article[i].src[j + 1], strlen(article[i].src) - j);
        name[strlen(name)] = '\0';
        break;
      } 
    }
    //strcpy(article[i].dst, dstdir);

    /*if(article[i].dst[strlen(dstdir) - 1] != '/') {
      article[i].dst[strlen(dstdir)] = '/';
      article[i].dst[strlen(dstdir) + 1] = '\0';
    }*/

    //int tmplen = strlen(article[i].dst);
    //strcpy(article[i].dst + strlen(article[i].dst), name);
    //int k = tmplen + strlen(name);
    
    k = strlen(name);
    name[k - 2] = 'h';
    name[k - 1] = 't';
    name[k] = 'm';
    name[k + 1] = 'l';
    name[k + 2] = '\0';

    strcpy(article[i].name, name);
  }
  free(name);
}

void copy_dirs(char *src_dir, int src_size, char *dst_dir) {
  char *get_dirs = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
  sprintf(get_dirs, get_dir_t, src_dir);
  char *dirs = exec_output(get_dirs);
  int k = 0;
  int beg = 0, end = 0;
  char *dir = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
  for(end = 0; end < strlen(dirs); end++) {
    if(dirs[end] == '\n') {
      memcpy(dir, &dirs[beg], end - beg);
      dir[end - beg] = '\0';
      char *copied_dir = get_dst(dir, src_dir, src_size, dst_dir);
      pid_t p = fork();
      if(p == -1) {
        fprintf(stderr, "csg: cannot fork process: %d\n", p);
        exit(1);
      } else if(p == 0) {
        execlp("cp", "cp", "-r", dir, copied_dir, NULL);
      }
      k++;
      beg = end + 1;
    }
  }
}

void rm_md_from_dir(char *src, char *dst) { /* removes all .md files from dst dir */
  if(strcmp(src, dst) == 0)
    return;
  char *rm_md = (char *) malloc(sizeof(char) * CSG_PATH_MAX);
  sprintf(rm_md, rm_md_t, dst);
  pid_t p = fork();
  if(p == -1) {
    fprintf(stderr, "csg: cannot fork process: %d\n", p);
    exit(1);
  } else if(p == 0) {
    execlp("sh", "sh", "-c", rm_md, NULL);
  }
}

void gen_pandoc_cmd(char *buff, struct art article) {
  if(strcmp(conf.art_header, "") == 0) {
    if(strcmp(conf.art_footer, "") == 0) {
      sprintf(buff, "pandoc -f markdown -t html -s %s -o %s --highlight-style="
          "%s --css=%s --template=%s", article.src, article.dst,
          conf.highlight_theme, conf.art_css, conf.art_template);
    } else {
      sprintf(buff, "pandoc -f markdown -t html -s -A %s %s -o %s"
          " --highlight-style=%s --css=%s --template=%s", conf.art_footer,
          article.src, article.dst, conf.highlight_theme, conf.art_css,
          conf.art_template);
    }
  } else {
    if(strcmp(conf.art_footer, "") == 0) {
      sprintf(buff, "pandoc -f markdown -t html -s -B %s %s -o %s"
          " --highlight-style=%s --css=%s --template=%s", conf.art_header,
          article.src, article.dst, conf.art_css, conf.highlight_theme,
          conf.art_template);
    } else {
      sprintf(buff, "pandoc -f markdown -t html -s -B %s -A %s %s -o %s"
          " --highlight-style=%s --css=%s --template=%s", conf.art_header,
          conf.art_footer, article.src, article.dst, conf.highlight_theme,
          conf.art_css, conf.art_template);
    }
  }
}

/* convert md to html */
void convert(struct art article) {
  char *convert = (char *) malloc(sizeof(char) * CSG_ARG_MAX);
  gen_pandoc_cmd(convert, article);

  printf("converting %s:\n%s\n\n", article.src, convert);

  pid_t p = fork();

  if(p == -1) {
    free(convert);
    fprintf(stderr, "csg: cannot create pid: %d\n", p);
    exit(1);
  } else if(p == 0){
    free(convert);
    execlp("sh", "sh", "-c", convert, NULL);
  } else return;
}

/* read main page header/footer */
char *read_mp (char *filename) {
  FILE *f = fopen(filename, "r");

  if(!f) {
    fprintf(stderr, "csg: cannot open file %s for reading\n", filename);
    exit(1);
  }

  char *buff = (char *) malloc(sizeof(char) * PIPE_BUF);
  int i = 0;

  while((buff[i] = fgetc(f))!= EOF) {
    i++;
  }

  buff[i] = '\0';
  return buff;
}

void gen_main_page(int nart, struct art *article, int src_size, char *path) {
  printf("generating %s\n", path);
  char *hdata = read_mp(conf.mp_header);
  char *fdata = read_mp(conf.mp_footer);

  FILE *index = fopen(path, "w");

  fprintf(index, "<!DOCTYPE html>\n<head>\n<meta charset=\"UTF-8\">\n");
  fprintf(index, "<meta name=\"viewport\" content=\"width=device-width"
      " initial-scale=1.0\">\n");
  fprintf(index, "<title>%s</title>\n", conf.mp_title);
  fprintf(index, "<link rel=\"stylesheet\" href=\"%s\">\n</head>\n", conf.mp_css);
  fprintf(index, "<body>\n<div class=\"container\">\n");

  if(hdata)
    fprintf(index, "%s", hdata);

  int i;
  for(i = 0; i < nart; i++) {
    fprintf(index, "<li class=\"item\">\n");
    char *rpath = get_relative_path(src_size, article[i].dst);
    if(rpath){
      fprintf(index, "<a href=\"%s\" class=\"title\">%s</a>\n", rpath + 1,
              article[i].title);
      free(rpath);
    }
    fprintf(index, "<div class=\"date\">%s</div>\n", article[i].date);
    fprintf(index, "<li/>\n");
  }

  if(fdata)
    fprintf(index, "%s", fdata);
  fprintf(index, "</div></body>");

  if(hdata)
    free(hdata);
  if(fdata)
    free(fdata);
}

int main(int argc, char **argv) {
  if(argc != 3) {
    printf("usage: %s src dst\n", argv[0]);
    printf("\tsrc: directory with Markdown files\n");
    printf("\tdst: destination directory for converted files\n");
    exit(1);
  }

  init_config_values(&conf, conf_val);
  open_config(&conf, conf_val);

  /* list all .md files in src directory */
  char *list_md = (char *) malloc(sizeof(char) * CSG_ARG_MAX);
  sprintf(list_md, list_md_t, argv[1]);

  char *mp = (char *) malloc(sizeof(char) * CSG_PATH_MAX);

  realpath(argv[2], mp);
  int tmplen = strlen(mp);
  mp[tmplen] = '/';
  mp[tmplen + 1] = '\0';
  strcpy(mp + strlen(mp), conf.mp_name);
  mp[strlen(mp)] = '\0';
  char *articles = exec_output(list_md);
  int nart = get_nart(articles, strlen(articles));

  struct art article[nart];
  parse_articles(article, strlen(articles), articles);
  get_info(nart, article);

  int i;
  for(i = 0; i < nart; i++) {
    if(strcmp(article[i].title, "\0") == 0)
      fprintf(stderr, "csg: missing title for %s!\n", article[i].src);
    if(strcmp(article[i].date, "\0") == 0)
      fprintf(stderr, "csg: missing date for %s!\n", article[i].src);
  }

  gen_dst(nart, article, argv[1], strlen(argv[1]), argv[2]);
 
  copy_dirs(argv[1], strlen(argv[1]), argv[2]); 
  rm_md_from_dir(argv[1], argv[2]);

  for(i = 0; i < nart; i++) {
    convert(article[i]);
  }

  gen_main_page(nart, article, strlen(argv[1]), mp);

  free(list_md);
  free(mp);

  return 0;
}
