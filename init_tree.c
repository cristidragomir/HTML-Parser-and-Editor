/* Dragomir Constantin-Cristian 314CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

#define MAX 1000
#define SIZE_CH sizeof(char)

TInfo InitInfo()
{
	TInfo new = malloc(sizeof(TNodInfo));
	new->type = NULL;
	new->id = NULL;
	new->style = NULL;
	new->otherAttributes = NULL;
	new->isSelfClosing = 0;
	new->contents = NULL;
	return new;
	// initializarea campului informatie din nod
}

TArb InitArb()
{
	TArb new = malloc(sizeof(TNodArb));
	if (!new) {
		return NULL;
	}
	new->info = NULL;
	new->nextSibling = NULL;
	new->firstChild = NULL;
	return new;
	// initializarea campurilor nodului
}

int InitAttrStyle(TAttr a)
{
	a->next = malloc(sizeof(TNodAttr));
	if (!a->next) {
		return 0;
	}
	a->next->name = NULL;
	a->next->value = NULL;
	a->next->next = NULL;
	return 1;
	// initializarea unui structuri pentru atributul style urmator
	// celui curent
}

int InitAttr(TArb a, char *name, char *value)
{
	int len_1, len_2;
	len_1 = strlen(name) + 1;
	len_2 = strlen(value) + 1;
	// lungimile sirurilor de caractere auxiliare
	TAttr prefix;
	if (strcmp(name, "style") == 0) {
		a->info->style = malloc(sizeof(TNodAttr));
		prefix = a->info->style;
		prefix->next = NULL;
		prefix->name = malloc(SIZE_CH * len_1);
		strcpy(prefix->name, name);
		prefix->value = malloc(SIZE_CH * len_2);
		strcpy(prefix->value, value);
		// modul de memorare al sirului de atribute
		// corespunzatoare atributului style in construirea arborelui
	} else {
		TAttr q;
		if (a->info->otherAttributes == NULL) {
			a->info->otherAttributes = malloc(sizeof(TNodAttr));
			q = a->info->otherAttributes;
		} else {
			q = a->info->otherAttributes;
			for(; q->next; q = q->next);
			q->next = malloc(sizeof(TNodAttr));
			q = q->next;
		}
		// cautarea ultimului atribut
		// initializarea unei noi structuri in lista
		q->next = NULL;
		q->name = malloc(SIZE_CH * len_1);
		q->value = malloc(SIZE_CH * len_2);
		strcpy(q->name, name);
		strcpy(q->value, value);
		// completarea campurilor
	}
	return 1;
}