#include <stdlib.h>
#include <string.h>

#include "util.h"

void swap(struct art *first, struct art *second)
{
	struct art *tmp = (struct art *) malloc(sizeof(struct art));
	strcpy(tmp->title, second->title);
	strcpy(tmp->date, second->date);
	strcpy(tmp->name, second->name);
	strcpy(tmp->src, second->src);
	strcpy(tmp->dst, second->dst);

	strcpy(second->title, first->title);
	strcpy(second->date, first->date);
	strcpy(second->name, first->name);
	strcpy(second->src, first->src);
	strcpy(second->dst, first->dst);

	strcpy(first->title, tmp->title);
	strcpy(first->date, tmp->date);
	strcpy(first->name, tmp->name);
	strcpy(first->src, tmp->src);
	strcpy(first->dst, tmp->dst);
}

void sort_articles(int nart, struct art *article)
{
	int i = 0, j = 0;
	for (; i < nart; i++) {
		for (j = i + 1; j < nart; j++) {
			if(strcmp(article[i].date, article[j].date) <= 0)
				swap(&article[i], &article[j]);
		}
	}
}
