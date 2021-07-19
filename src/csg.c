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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __linux__
#include <linux/limits.h>
#endif

#ifdef __FreeBSD__
#include <sys/syslimits.h>
#endif

#ifdef __APPLE__
#include <sys/syslimits.h>
#endif

#include "csg.h"
#include "config.h"

char *exec_output(char *cmd) {
  char *output = (char *) malloc(sizeof(char) * ARG_MAX);
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
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
    execlp("sh", "sh", "-c", cmd, NULL);
    exit(EXIT_FAILURE);
  } else {
    close(fd[1]);
    read(fd[0], output, ARG_MAX);
  }

  output[strlen(output)] = '\0'; /* don't need \n */

  return output;
}

int get_nart(char *buff, int size) {
  int i;
  int cnt = 0;

  for(i = 0; i < size; i++) {
    if(buff[i] == '\n' || buff[i] == '\0')
      cnt++;
  }

  return cnt;
}

void parse(art *article, int size, char *buff) {
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

void get_info(int nart, art *article) { /* get title and date */
  for(int i = 0; i < nart; i++) {
    char *get_title = (char *) malloc(sizeof(char) * NAME_MAX);
    char *get_date = (char *) malloc(sizeof(char) * NAME_MAX);

    sprintf(get_title, get_title_t, article[i].src);
    sprintf(get_date, get_date_t, article[i].src);

    char *title = exec_output(get_title);
    char *date = exec_output(get_date);

    //free(get_title);
    //free(get_date);

    title[strlen(title) - 1]  = '\0'; /* we don't need both \n and \0 */
    date[strlen(date) - 1]  = '\0'; /* the same */

    strcpy(article[i].title, title);
    strcpy(article[i].date, date);

    //free(title);
    //free(date);
  }
}

void gen_dst(int nart, art *article, char *dstdir) { /* generate dst path */
  char *name = (char *) malloc(sizeof(char) * NAME_MAX);
  int i, j;

  for(i = 0; i < nart; i++) {
    for(j = strlen(article[i].src) - 1; j >= 0; j--) {
      if(article[i].src[j] == '/') {
        memcpy(name, &article[i].src[j + 1], strlen(article[i].src) - j);
        name[strlen(name)] = '\0';
        break;
      } 
    }

    strcpy(article[i].dst, dstdir);

    if(article[i].dst[strlen(dstdir) - 1] != '/') {
      article[i].dst[strlen(dstdir)] = '/';
      article[i].dst[strlen(dstdir) + 1] = '\0';
    }

    int tmplen = strlen(article[i].dst);
    strcpy(article[i].dst + strlen(article[i].dst), name);
    int k = tmplen + strlen(name);

    article[i].dst[k - 2] = 'h';
    article[i].dst[k - 1] = 't';
    article[i].dst[k] = 'm';
    article[i].dst[k + 1] = 'l';
    article[i].dst[k + 2] = '\0';
  }
}

void gen_pandoc_cmd(char *buff, char *article_header, char *article_footer,
                    art article, char *article_css, char *article_template) {
  if(strcmp(article_header, "") == 0) {
    if(strcmp(article_footer, "") == 0) {
      sprintf(buff, "pandoc -f markdown -t html -s %s -o %s --highlight-style"
              " breezedark --css=%s --template=%s", article.src, article.dst,
              article_css, article_template);
    } else {
      sprintf(buff, "pandoc -f markdown -t html -s -A %s %s -o %s"
              " --highlight-style breezedark --css=%s --template=%s",
              article_footer, article.src, article.dst, article_css,
              article_template);
    }
  } else {
    if(strcmp(article_footer, "") == 0) {
      sprintf(buff, "pandoc -f markdown -t html -s -B %s %s -o %s"
              " --highlight-style breezedark --css=%s --template=%s",
              article_header, article.src, article.dst, article_css,
              article_template);
    } else {
      sprintf(buff, "pandoc -f markdown -t html -s -B %s -A %s %s -o %s"
              " --highlight-style breezedark --css=%s --template=%s",
              article_header, article_footer, article.src, article.dst,
              article_css, article_template);
    }
  }
}

void convert(art article, char *article_css, char *article_template,
             char *article_header, char *article_footer) {
  char *convert = (char *) malloc(sizeof(char) * ARG_MAX);
  gen_pandoc_cmd(convert, article_header, article_footer, article, article_css,
      article_template);

  printf("%s\n", convert);

  pid_t p = fork();

  if(p == -1) {
    fprintf(stderr, "csg: cannot create pid: %d\n", p);
    exit(1);
  } else if(p == 0){
    execlp("sh", "sh", "-c", convert, NULL);
  } else return;
}

char *read_mp (char *filename) { /* reads mainpage header/footer */
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

void gen_main_page(int nart, art *article, char *path, char *css,
                   char *mp_header, char *mp_footer) {
  char *hdata = read_mp(mp_header);
  char *fdata = read_mp(mp_footer);

  FILE *index = fopen(path, "w");

  fprintf(index, "<!DOCTYPE html>\n<head>\n<meta charset=\"UTF-8\">\n");
  fprintf(index, "<meta name=\"viewport\" content=\"width=device-width"
          " initial-scale=1.0\">\n");
  fprintf(index, "<title>%s</title>\n", mp_title);
  fprintf(index, "<link rel=\"stylesheet\" href=\"%s\">\n</head>\n", css);
  fprintf(index, "<body>\n<div class=\"container\">\n");

  if(hdata)
    fprintf(index, "%s", hdata);

  int i;
  for(i = 0; i < nart; i++) {
    fprintf(index, "<li class=\"item\">\n");
    fprintf(index, "<a href=\"%s\" class=\"title\">%s</a>\n", article[i].dst,
            article[i].title);
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

  /* list all .md files in src directory */
  char *list_md = (char *) malloc(sizeof(char) * ARG_MAX);
  sprintf(list_md, list_md_t, argv[1]);

  char *article_css = (char *) malloc(sizeof(char) * PATH_MAX);
  char *article_template = (char *) malloc(sizeof(char) * PATH_MAX);
  char *article_header = (char *) malloc(sizeof(char) * PATH_MAX);
  char *article_footer = (char *) malloc(sizeof(char) * PATH_MAX);

  char *mp_css = (char *) malloc(sizeof(char) * PATH_MAX);
  char *mp_header = (char *) malloc(sizeof(char) * PATH_MAX);
  char *mp_footer = (char *) malloc(sizeof(char) * PATH_MAX);
  char *mp = (char *) malloc(sizeof(char) * PATH_MAX);

  sprintf(article_css, article_css_t, confloc);
  sprintf(article_template, article_template_t, confloc); 
  sprintf(article_header, article_header_t, confloc);
  sprintf(article_footer, article_footer_t, confloc);

  sprintf(mp_css, mp_css_t, confloc);
  sprintf(mp_header, mp_header_t, confloc);
  sprintf(mp_footer, mp_footer_t, confloc);
  realpath(argv[2], mp);

  mp[strlen(mp)] = '/';
  strcpy(mp + strlen(mp), mp_name_t);

  char *articles = exec_output(list_md);
  int nart = get_nart(articles, strlen(articles));

  art article[nart];
  parse(article, strlen(articles), articles);
  get_info(nart, article);

  int i;
  for(i = 0; i < nart; i++) {
    if(strcmp(article[i].title, "\0") == 0)
      fprintf(stderr, "csg: missing title for %s!\n", article[i].src);
    if(strcmp(article[i].date, "\0") == 0)
      fprintf(stderr, "csg: missing date for %s!\n", article[i].src);
  }

  gen_dst(nart, article, argv[2]);

  for(i = 0; i < nart; i++) {
    convert(article[i], article_css, article_template, article_header, 
            article_footer);
  }

  gen_main_page(nart, article, mp, mp_css, mp_header, mp_footer);

  free(list_md);
  free(article_css);
  free(article_template);
  free(article_header);
  free(article_footer);
  free(mp_css);
  free(mp);
  free(mp_header);
  free(mp_footer);

  return 0;
}
