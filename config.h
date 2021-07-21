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

char *confloc = "."; /* config location, change it to absolute path 
                       * if you want to use csg outside of installation directory */
char *mp_title = "csg"; /* mainpage title */
char *mp_css_t = "%s/css/index.css"; /* mainpage css */
char *mp_header_t = "%s/html/mpheader.html"; /* mainpage header */
char *mp_footer_t = "%s/html/mpfooter.html"; /* mainpage footer */
char *mp_name_t = "index.html"; /* mainpage name */

char *article_css_t = "%s/css/pandoc.css"; /* css for article*/
char *article_template_t = "%s/html/template.html"; /* template for article */
char *article_header_t = "%s/html/header.html"; /* header for the article */
char *article_footer_t = "%s/html/footer.html"; /* footer for the article */

char *list_md_t = "find %s | grep '.md$'"; /* list all .md files*/
char *make_dir_t = "mkdir -p %s"; /* make dst dir */
char *get_title_t = "cat %s | grep title | cut -c 8-"; /* 'title: ' = 7 */
char *get_date_t = "cat %s | grep date | cut -c 7-"; /* 'date: ' = 6 */
