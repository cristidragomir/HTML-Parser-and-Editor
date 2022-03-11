/* Dragomir Constantin-Cristian 314CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"
#include "init_tree.h"
#include "tree_delete.h"

#define MAX 1000
#define SIZE_CH sizeof(char)

void modify_tag_style_mode_1 (int *j, char *strToComplete, 
	TAttr *aux, TAttr *ant, char **name, char **val)
{
	(*val)[*j] = '\0';
	*j = 0;
	*strToComplete = 1;
	(*aux)->name = malloc(SIZE_CH * (strlen(*name)+1));
	(*aux)->value = malloc(SIZE_CH * (strlen(*val) + 1));
	strcpy((*aux)->name, *name);
	strcpy((*aux)->value, *val);
	free(*name);
	free(*val);
	*name = malloc(SIZE_CH * MAX);
	*val = malloc(SIZE_CH * MAX);
	InitAttrStyle(*aux);
	if (*ant != *aux) {
		*ant = *aux;
	}
	*aux = (*aux)->next;
	// modificarea atributului in modul append
}

void modify_tag_style_aux (TArb q, char *style)
{
	if (q->info->style) {
		freeAttr(q->info->style);
	}
	q->info->style = malloc(sizeof(TNodAttr));
	q->info->style->next = NULL;
	TAttr aux = q->info->style;
	TAttr ant = aux;
	char *beginQuote = strchr(style, '\"');
	beginQuote++;
	char *endQuote = strchr(beginQuote, '\"');
	char *name = malloc(SIZE_CH * MAX); 
	char *val = malloc(SIZE_CH * MAX);
	char strToComplete = 1;
	int j = 0;
	while (beginQuote < endQuote) {
		if (strToComplete == 1) {
			if (*beginQuote != ' ' && *beginQuote != ':') {
				name[j++] = *beginQuote;
			}
			if (*beginQuote == ':') {
				name[j] = '\0';
				j = 0;
				strToComplete = 2;
			}
		} else if (strToComplete == 2) {
			if (*beginQuote != ' ' && *beginQuote != ';') {
				val[j++] = *beginQuote;
			}
			if (*beginQuote == ';') {
				modify_tag_style_mode_1(&j, &strToComplete, 
					&aux, &ant, &name, &val);
			}
		}
		beginQuote++;
	}
	ant->next = NULL;
	freeAttr(aux);
	if (name) {
		free(name);
	}
	if (val) {
		free(val);
	}
	// preluarea numelui de atribut si a valorii sale
}

void modify_tag_style_mode_2 (TArb q, int *j, char *strToComplete, 
	TAttr *aux, char **name, char **val)
{
	(*val)[*j] = '\0';
	int found = 0;
	*j = 0;
	*strToComplete = 1;
	while (*aux) {
		if (strcmp((*aux)->name, *name) == 0) {
			found = 1;
			free((*aux)->value);
			(*aux)->value = malloc(SIZE_CH * (strlen(*val) + 1));
			strcpy((*aux)->value, *val);	
		}
		if ((*aux)->next) {
			*aux = (*aux)->next;
		} else {
			break;
		}
	}
	if (!found) {
		if (*aux) {
			InitAttrStyle(*aux);
			*aux = (*aux)->next;
		} else {
			q->info->style = malloc(sizeof(TNodAttr));
			q->info->style->next = NULL;
			*aux = q->info->style;
		}
		(*aux)->name = malloc(SIZE_CH * (strlen(*name) + 1));
		(*aux)->value = malloc(SIZE_CH * (strlen(*val) + 1));
		strcpy((*aux)->name, *name);
		strcpy((*aux)->value, *val);
	}
	free(*name);
	free(*val);
	*name = malloc(SIZE_CH * MAX);
	*val = malloc(SIZE_CH * MAX);
	*aux = q->info->style;
	// modificarea atributelor in modul append
}

void modify_tag_style(TArb q, char *style, int mode) 
{
	if (mode == 1) {
		// overrideStyle
		modify_tag_style_aux (q, style);
		return;
	}
	if (mode != 2) {
		return;
	}
	// append
	TAttr aux = q->info->style;
	char *beginQuote = strchr(style, '\"');
	beginQuote++;
	char *endQuote = strchr(beginQuote, '\"');
	char *name = malloc(SIZE_CH * MAX); 
	char *val = malloc(SIZE_CH * MAX);
	char strToComplete = 1;
	int j = 0;
	while (beginQuote < endQuote) {
		if (strToComplete == 1) {
			if (*beginQuote != ' ' && *beginQuote != ':') {
				name[j++] = *beginQuote;
			}
			if (*beginQuote == ':') {
				name[j] = '\0';
				j = 0;
				strToComplete = 2;
			}
		} else if (strToComplete == 2) {
			if (*beginQuote != ' ' && *beginQuote != ';') {
				val[j++] = *beginQuote;
			}
			if (*beginQuote == ';') {
				modify_tag_style_mode_2(q, &j, &strToComplete, 
					&aux, &name, &val);
			}
		}
		beginQuote++;
	}
	if (name) {
		free(name);
	}
	if (val) {
		free(val);
	}
	// modificarea efectiva a atribtului style
	// existand doua moduri: 1 - override, 2 - append
}

void find_tag_class(TArb a, char *className, char *style,
	int *found, int mode)
{
	TArb q = a->firstChild;
	while (q) {
		TAttr p = q->info->otherAttributes;
		while (p) {
			if (strcmp(p->name, "class") == 0 
				&& strcmp(p->value, className) == 0) {
				break;
			}
			p = p->next;
		}
		if (p) {
			*found = 1;
			modify_tag_style(q, style, mode);
		}
		q = q->nextSibling;
	}
	if (a->nextSibling) {
		find_tag_class(a->nextSibling, className, style, found, mode);
	}
	if (a->firstChild) {
		find_tag_class(a->firstChild, className, style, found, mode);
	}
	// cautarea tagurilor dupa o anumita clasa
}

void find_tag_name(TArb a, char *tagName, char *style, int *found, int mode)
{
	TArb q = a->firstChild;
	while (q) {
		if (strcmp(q->info->type, tagName) == 0) {
			*found = 1;
			modify_tag_style(q, style, mode);
		}
		q = q->nextSibling;
	}
	if (a->nextSibling) {
		find_tag_name(a->nextSibling, tagName, style, found, mode);
	}
	if (a->firstChild) {
		find_tag_name(a->firstChild, tagName, style, found, mode);
	}
	// cautarea unui tag dupa numele/tipul sau
}

void find_tag_class_name(TArb a, char *tagName, char *className,
	char *style, int *found, int mode)
{
	TArb q = a->firstChild;
	while (q) {
		if (strcmp(q->info->type, tagName) == 0) {
			TAttr p = q->info->otherAttributes;
			while (p) {
				if (strcmp(p->name, "class") == 0 
					&& strcmp(p->value, className) == 0) {
					break;
				}
				p = p->next;
			}
			if (p) {
				*found = 1;
				modify_tag_style(q, style, mode);
			}
		}
		q = q->nextSibling;
	}
	if (a->nextSibling) {
		find_tag_class_name(a->nextSibling, tagName, className,
			style, found, mode);
	}
	if (a->firstChild) {
		find_tag_class_name(a->firstChild, tagName, className,
			style, found, mode);
	}
	// cautarea unui anumit tag dupa nume si o anumita clasa
}

void find_tag_parent_child(TArb a, char *tagParent, char *tagChild,
	char *style, int *found, int mode)
{
	TArb q = a->firstChild;
	while (q) {
		if (strcmp(a->info->type, tagParent) == 0
			&& strcmp(q->info->type, tagChild) == 0) {
			*found = 1;
			modify_tag_style(q, style, mode);
		}
		q = q->nextSibling;
	}
	if (a->nextSibling) {
		find_tag_parent_child(a->nextSibling, tagParent, tagChild,
			style, found, mode);
	}
	if (a->firstChild) {
		find_tag_parent_child(a->firstChild, tagParent, tagChild,
			style, found, mode);
	}
	// cautarea unui anumit tag care are un anumit parinte
}

void find_tag_ancestor_child(TArb a, char *tagAncestor, char *tagChild,
	int ancestorFound, char *style, int *found, int mode)
{
	TArb q = a->firstChild;
	if (strcmp(a->info->type, tagAncestor) == 0) {
		ancestorFound = 1;
	}
	while (q) {
		if (strcmp(q->info->type, tagChild) == 0 && ancestorFound) {
			*found = 1;
			modify_tag_style(q, style, mode);
		}
		q = q->nextSibling;
	}
	if (a->nextSibling) {
		find_tag_ancestor_child(a->nextSibling, tagAncestor, tagChild,
			ancestorFound, style, found, mode);
	}
	if (a->firstChild) {
		find_tag_ancestor_child(a->firstChild, tagAncestor, tagChild,
			ancestorFound, style, found, mode);
	}
	// cautarea unui tag cu un anumit stramos
}

void print_modif_error(FILE *out, char *arg, int mode, int type)
{
	if (mode == 1) {
		if (type == 1) {
			fprintf(out, "Override style failed");
		} else if (type == 2) {
			fprintf(out, "Append to style failed");	
		}
		fprintf(out, ": no node found for selector #%s!\n", arg);
	} else if (mode == 2) {
		if (type == 1) {
			fprintf(out, "Override style failed");
		} else if (type == 2) {
			fprintf(out, "Append to style failed");	
		}
		fprintf(out, ": no node found for selector %s!\n", arg);
	} else if (mode == 3) {
		if (type == 1) {
			fprintf(out, "Override style failed");
		} else if (type == 2) {
			fprintf(out, "Append to style failed");	
		}
			fprintf(out, ": no node found for selector .%s!\n", arg);
	}
	// afisarea unui mesaj de eroare atunci cand nu se gaseste un nod
	// dupa un anumit id
}

void modif_find_after_id(TArb a,char *style, char *sel_arg_cpy, int nr_pos, 
	int *positions, FILE *out, int type) 
{
	char found = 1;
	int i;
	TArb q = a->firstChild;
	if (!q) {
		found = 0;
		print_modif_error(out, sel_arg_cpy, 1, type);
	}
	int cnt;
	if (found)
	for (i = 0; i < nr_pos; i++) {
		for (cnt = 1; cnt < positions[i]; cnt++) {
			q = q->nextSibling;
			if (!q) {
				print_modif_error(out, sel_arg_cpy, 1, type);
				found = 0;
				break;
			}
		}
		if (i != nr_pos - 1 && found) {
			q = q->firstChild;
			if (!q) {
				print_modif_error(out, sel_arg_cpy, 1, type);
				found = 0;
				break;
			}
		}
		if (!found) {
			break;
		}
	}
	if (found) {
		modify_tag_style(q, style, type);	
	}
	free(positions);
	free(sel_arg_cpy);
	// modificarea atributului unui tag gasit dupa id
}
void split_id_style (char *sel_arg, int **positions,
	int *nr_pos, char **sel_arg_cpy) 
{
	int len = strlen(sel_arg);
	int i;
	for (i = 0; i < len; i++) {
		if (sel_arg[i] == '.') {
			(*nr_pos)++;
		}
	}
	*sel_arg_cpy = malloc(SIZE_CH * (len + 1));
	strcpy(*sel_arg_cpy, sel_arg);
	*positions = malloc(sizeof(int) * (*nr_pos));
	char *nr;
	nr = strtok(sel_arg, ".");
	i = 0;
	while (nr) {
		(*positions)[i++] = atoi(nr);
		nr = strtok(NULL, ".");
	}
	// transformarea in int a numerelor incadrate intre puncte
}

void determine_between_symbol_style(char *symbol, char *sel, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		if (sel[i] == '.') {
			*symbol = '.';
			break;
		}
		if (sel[i] == ' ') {
			*symbol = ' ';
			break;
		}
		if (sel[i] == '>') {
			*symbol = '>';
			break;
		}
	}
	// determinarea selectorului
}

void modif_el_point_el(TArb a, char *style, char *sel, 
	int len, FILE *out, int mode)
{
	int found = 0;
	char *tagName;
	char *className;
	tagName = malloc(SIZE_CH * (len + 1));
	className = malloc(SIZE_CH * (len + 1));
	char *sel_cpy = malloc(SIZE_CH * (len + 1));
	strcpy(sel_cpy, sel);
	char *aux;
	aux = strtok(sel_cpy, ".");
	strcpy(tagName, aux);
	aux = strtok(NULL, ".");
	strcpy(className, aux);
	find_tag_class_name(a, tagName, className, style, &found, mode);
	if (!found) {
		print_modif_error(out, sel, 2, mode);
	}
	free(tagName);
	free(className);
	free(sel_cpy);
	// modificarea unui atribut cu un anumit nume si o
	// anumita clasa
}

void modif_el_greater_el (TArb a, char *style, char *sel, 
	int len, FILE *out, int mode)
{
	int found = 0;
	char *tagParent;
	char *tagChild;
	tagParent = malloc(SIZE_CH * (len + 1));
	tagChild = malloc(SIZE_CH * (len + 1));
	char *sel_cpy = malloc(SIZE_CH * (len + 1));
	strcpy(sel_cpy, sel);
	char *aux;
	aux = strtok(sel_cpy, ">");
	strcpy(tagParent, aux);
	aux = strtok(NULL, ">");
	strcpy(tagChild, aux);
	find_tag_parent_child(a, tagParent, tagChild, style, &found, mode);
	if (!found) {
		print_modif_error(out, sel, 2, mode);
	}
	free(tagParent);
	free(tagChild);
	free(sel_cpy);
	// modificarea atributului unui tag care are ca parinte un tag specific
}

void modif_el_space_el (TArb a, char *style, char *sel, 
	int len, FILE *out, int mode)
{
	int found = 0;
	char *tagAncestor;
	char *tagChild;
	tagAncestor = malloc(SIZE_CH * (len + 1));
	tagChild = malloc(SIZE_CH * (len + 1));
	char *sel_cpy = malloc(SIZE_CH * (len + 1));
	strcpy(sel_cpy, sel);
	char *aux;
	aux = strtok(sel_cpy, " ");
	strcpy(tagAncestor, aux);
	aux = strtok(NULL, " ");
	strcpy(tagChild, aux);
	find_tag_ancestor_child(a, tagAncestor, tagChild, 0, style, &found, mode);
	if (!found) {
		print_modif_error(out, sel, 2, mode);
	}
	free(tagAncestor);
	free(tagChild);
	free(sel_cpy);
	// modificarea atributului style pentru un tag al carui
	// stramos e un tag specific
}

void style_tool(TArb a, char *selector, char *style, FILE *out, int mode)
{
	int start;
	char *sel_arg;
	int len = strlen(selector);
	for (start = 0; start < len && selector[start] != '\"'; start++);
	start++;
	sel_arg = malloc(SIZE_CH * (len - start));
	strcpy(sel_arg, &selector[start + 1]);
	sel_arg[len - start - 2] = '\0';
	if (selector[start] == '#') {
		int nr_pos = 1;
		int *positions = NULL;
		char *sel_arg_cpy = NULL;
		split_id_style(sel_arg, &positions, &nr_pos, &sel_arg_cpy);
		modif_find_after_id(a, style, sel_arg_cpy, 
			nr_pos, positions, out, mode);
	} else if (selector[start] == '.') {
		int found = 0;
		find_tag_class(a, sel_arg, style, &found, mode);
		if (!found) {
			print_modif_error(out, sel_arg, 3, mode);
		}
	} else {
		len = strlen(selector + start);
		char *sel = malloc(SIZE_CH * (len + 1));
		strcpy(sel, selector + start);
		sel[len - 1] = '\0';
		len--;
		
		char symbol = '\0';
		determine_between_symbol_style(&symbol, sel, len);
		if (symbol == '\0') {
			int found = 0;
			find_tag_name(a, sel, style, &found, mode);
			if (!found) {
				print_modif_error(out, sel_arg, 3, mode);
			}
		} else if (symbol == '.') {
			modif_el_point_el(a, style, sel, len, out, mode);
		} else if (symbol == '>') {
			modif_el_greater_el(a, style, sel, len, out, mode);
		} else if (symbol == ' ') {
			modif_el_space_el(a, style, sel, len, out, mode);
		}
		free(sel);
	}
	free(sel_arg);
	// cele doua functii append si override sunt implemntate aici
	// in functie de selectorul dat de la tastatura
	// se apeleaza o functie specializata	
}