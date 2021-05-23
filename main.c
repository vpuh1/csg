#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* const char* const substr(char* s, size_t pos, size_t count) {
   static char buf[BUFSIZ];
   buf[sizeof(buf) - 1] = '\0';
   if ( count >= BUFSIZ )
      return NULL;
   else
      return strncpy(buf, s + pos, count);
} */

char output[256];

int main() {
	int fd[2];
	char *listdoc[] = { "sh", "-c", "ls | grep .md", NULL};
	char *sum[] = { "touch", "summary.html", NULL };
	if(pipe(fd) == -1) {
		fprintf(stderr, "An error occurued creating pipe\n");
		return 0;
	}
	pid_t pid = fork();
	if(pid == -1) {
		fprintf(stderr, "An error occurued creating process: %d\n", pid);
		return 0;
	}
	if(pid == 0) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execvp(listdoc[0], listdoc);
		exit(EXIT_FAILURE);
	}
	else {
		close(fd[1]);
		read(fd[0], output, sizeof(output));
		//printf("%s\n", output);
	}
	output[strlen(output)] = '\0';
	int mdn = 0;
	for(int i = 0; i < strlen(output); i++) {
		if(output[i] == '\n')
			mdn++;
	}
	char doc[mdn][256];
	char title[mdn][256];
	int k = 0;
	int sb = 0, se=0;
	for(int i = 0; i < strlen(output); i++) {
		if(output[i] == '\n') {
			se = i;
			if(k != 3) {
				strncpy(doc[k], output+sb, se-sb);
			doc[k][se-sb] = '\0';
			//printf("%s\n", doc[k]);
			sb = i+1;
			k++;
			}
		}
	}
	//printf("%d\n", mdn);
	int fd2[2];
	for(int i = 0; i < 255; i++) {
		output[i] = 0;
	}
	output[255] = '\0';
	for(int i = 0; i < mdn; i++) {
		if(pipe(fd2) == -1) {
			fprintf(stderr, "An error occurued creating pipe\n");
			return 0;
		}
		pid_t p = fork();
		if(p == -1) {
			fprintf(stderr, "An error occurued creating process: %d\n", pid);
			return 0;
		}
		if(p == 0){
			dup2(fd2[1], STDOUT_FILENO);
			close(fd2[0]);
			close(fd2[1]);
			char arg[256];
			strcpy(arg, "cat ");
			strncat(arg, doc[i], strlen(doc[i]));
			strcat(arg, " | grep title | cut -c 8-");
			//printf("%s\n", arg);
			execlp("sh", "sh", "-c", arg, NULL);
			exit(EXIT_FAILURE);
		}
		else {
			close(fd2[1]);
			read(fd2[0], title[i], sizeof(title[i]));
			//printf("%d\n", strlen(title[i]));
			printf("%s\n", title[i]);
		}
		title[i][strlen(title[i])-2] = '\0';
	}
	pid_t pid2 = fork();
	if(pid2 == -1) {
		fprintf(stderr, "An error occurued creating process: %d\n", pid);
		return 0;
	}
	if(pid2 == 0) {
		execvp(sum[0], sum);
	}
	FILE *summary;
	summary = fopen("summary.html", "w");
	fprintf(summary, "<ul>\n");
	for(int i = 0; i < k; i++) {
		fprintf(summary, "    <li><a href=\"./%s\">%s</a></li>\n", doc[i], title[i]);
	}
	fprintf(summary, "</ul>");
	return 0;
}

