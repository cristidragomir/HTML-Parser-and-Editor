/* Dragomir Constantin-Cristian 314CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

#define MAX 1000
#define SIZE_CH sizeof(char)

void freeAttr(TAttr attrNode)
{
	TAttr advance = attrNode;
	TAttr current = advance;
	while (advance) {
		advance = advance->next;
		free(current->name);
		free(current->value);
		free(current);
		current = advance;
	}
	// eliminarea listei de atribute
}

void freeInfo(TInfo infoNode)
{
	free(infoNode->type);
	free(infoNode->id);
	if (infoNode->contents) {
		free(infoNode->contents);
		infoNode->contents = NULL;
	}
	if (infoNode->style) {
		freeAttr(infoNode->style);
		infoNode->style = NULL;
	}
	if (infoNode->otherAttributes) {
		freeAttr(infoNode->otherAttributes);
		infoNode->otherAttributes = NULL;
	}
	infoNode->style = NULL;
	infoNode->otherAttributes = NULL;
	free(infoNode);
	// eliberarea structurii de informatii
}

void delete_tree_part(TArb a)
{
	if (a->nextSibling) {
		delete_tree_part(a->nextSibling);
	}
	if (a->firstChild) {
		delete_tree_part(a->firstChild);
	}
	freeInfo(a->info);
	free(a);
	// stergerea nodurilor din arbore incepand cu un anumit nod
}

void find_del_tag_class(TArb a, char *className, int *found)
{
	TArb q = a->firstChild;
	TArb ant = a;
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
			if (ant->firstChild == q) {
				ant->firstChild = q->nextSibling;
			} else if (ant->nextSibling == q) {
				ant->nextSibling = q->nextSibling;
			}
			*found = 1;
			q->nextSibling = NULL;
			delete_tree_part(q);
			q = ant;
		}
		if (ant != q) {
			ant = q;
		}
		if (q != a) {
			q = q->nextSibling;
		} else {
			q = q->firstChild;
		}
	}
	if (a->nextSibling) {
		find_del_tag_class(a->nextSibling, className, found);
	}
	if (a->firstChild) {
		find_del_tag_class(a->firstChild, className, found);
	}
	// se cauta si se sterg toate tagurile care apartin unei anumite clase
}

void find_del_tag_name(TArb a, char *tagName, int *found)
{
	TArb q = a->firstChild;
	TArb ant = a;
	while (q) {
		if (strcmp(q->info->type, tagName) == 0) {
			if (ant->firstChild == q) {
				ant->firstChild = q->nextSibling;
			} else if (ant->nextSibling == q) {
				ant->nextSibling = q->nextSibling;
			}
			*found = 1;
			q->nextSibling = NULL;
			delete_tree_part(q);
			q = ant;
		}
		if (ant != q) {
			ant = q;
		}
		if (q != a) {
			q = q->nextSibling;
		} else {
			q = q->firstChild;
		}
	}
	if (a->nextSibling) {
		find_del_tag_name(a->nextSibling, tagName, found);
	}
	if (a->firstChild) {
		find_del_tag_name(a->firstChild, tagName, found);
	}
	// stergerea tuturor tagurilor care au un anumit nume
}

void del_tag_class_name_aux (TAttr *p, char *className, 
	TArb *q, TArb *ant, int *found)
{
	while (*p) {
		if (strcmp((*p)->name, "class") == 0 
			&& strcmp((*p)->value, className) == 0) {
			break;
		}
		(*p) = (*p)->next;
	}
	if (*p) {
		if ((*ant)->firstChild == *q) {
			(*ant)->firstChild = (*q)->nextSibling;
		} else if ((*ant)->nextSibling == *q) {
			(*ant)->nextSibling = (*q)->nextSibling;
		}
		*found = 1;
		(*q)->nextSibling = NULL;
		delete_tree_part(*q);
		*q = *ant;
	}
	// stergerea unui tag cu un anumit nume si o
	// anumita clasa din care face parte
}

void find_del_tag_class_name(TArb a, char *tagName, 
	char *className, int *found)
{
	TArb q = a->firstChild;
	TArb ant = a;
	while (q) {
		if (strcmp(q->info->type, tagName) == 0) {
			TAttr p = q->info->otherAttributes;
			del_tag_class_name_aux(&p, className, &q, &ant, found);
		}
		if (ant != q) {
			ant = q;
		}
		if (q != a) {
			q = q->nextSibling;
		} else {
			q = q->firstChild;
		}
	}
	if (a->nextSibling) {
		find_del_tag_class_name(a->nextSibling, tagName, className, found);
	}
	if (a->firstChild) {
		find_del_tag_class_name(a->firstChild, tagName, className, found);
	}
	// stergerea tagurilor care au un anumit nume si o anumita clasa
}

void find_del_tag_parent_child(TArb a, char *tagParent, 
	char *tagChild, int *found)
{
	TArb q = a->firstChild;
	TArb ant = a;
	while (q) {
		if (strcmp(a->info->type, tagParent) == 0
			&& strcmp(q->info->type, tagChild) == 0) {
			if (ant->firstChild == q) {
				ant->firstChild = q->nextSibling;
			} else if (ant->nextSibling == q) {
				ant->nextSibling = q->nextSibling;
			}
			*found = 1;
			q->nextSibling = NULL;
			delete_tree_part(q);
			q = ant;
		}
		if (ant != q) {
			ant = q;
		}
		if (q != a) {
			q = q->nextSibling;
		} else {
			q = q->firstChild;
		}
	}
	if (a->nextSibling) {
		find_del_tag_parent_child(a->nextSibling, tagParent, tagChild, found);
	}
	if (a->firstChild) {
		find_del_tag_parent_child(a->firstChild, tagParent, tagChild, found);
	}
	// stergerea tagurilor care au ca parinte un anumit tag
}

void find_del_tag_ancestor_child(TArb a, char *tagAncestor, char *tagChild, 
	int ancestorFound, int *found)
{
	TArb q = a->firstChild;
	TArb ant = a;
	if (strcmp(a->info->type, tagAncestor) == 0) {
		ancestorFound = 1;
	}
	while (q) {
		if (strcmp(q->info->type, tagChild) == 0 && ancestorFound) {
			if (ant->firstChild == q) {
				ant->firstChild = q->nextSibling;
			} else if (ant->nextSibling == q) {
				ant->nextSibling = q->nextSibling;
			}
			*found = 1;
			q->nextSibling = NULL;
			delete_tree_part(q);
			q = ant;
		}
		if (ant != q) {
			ant = q;
		}
		if (q != a) {
			q = q->nextSibling;
		} else {
			q = q->firstChild;
		}
	}
	if (a->nextSibling) {
		find_del_tag_ancestor_child(a->nextSibling, tagAncestor, 
			tagChild, ancestorFound, found);
	}
	if (a->firstChild) {
		find_del_tag_ancestor_child(a->firstChild, tagAncestor, 
			tagChild, ancestorFound, found);
	}
	// stergerea tagurilor care au un anumit tag stramos
}

void create_ids_html_case (TArb a)
{
	if (a->info->id) {
			free(a->info->id);
	}
	a->info->id = malloc(SIZE_CH * 2);
	strcpy(a->info->id, "0");
	TArb q = a->firstChild;
	if (q->info->id) {
		free(q->info->id);
	}
	q->info->id = malloc(SIZE_CH * 2);
	strcpy(q->info->id, "1");
	if (q->nextSibling->info->id) {
		free(q->nextSibling->info->id);
	}
	q->nextSibling->info->id = malloc(SIZE_CH * 2);
	strcpy(q->nextSibling->info->id, "2");
	// crearea id-urilor pentru html, head, body
}

void create_ids_node(TArb a)
{
	TArb q = a->firstChild;
	int cnt = 1;
	int len = strlen(a->info->id);
	for (; q; q = q->nextSibling) {
		int cnt_cpy = cnt;
		int nr_cfr = 0;
		for (; cnt_cpy; nr_cfr++, cnt_cpy /= 10);
		if (q->info->id) {
			free(q->info->id);
		}
		q->info->id = malloc(SIZE_CH * (len + nr_cfr + 2));
		char *char_nr = malloc(SIZE_CH * (nr_cfr + 1));
		int nr_cfr_cpy = nr_cfr;
		char_nr[nr_cfr_cpy--] = '\0';
		cnt_cpy = cnt;
		while (nr_cfr_cpy >= 0) {
			char_nr[nr_cfr_cpy--] = (cnt_cpy % 10) + '0';
			cnt_cpy /= 10;
		}
		strcpy(q->info->id, a->info->id);
		strcat(q->info->id, ".");
		strcat(q->info->id, char_nr);
		free(char_nr);
		cnt++;
	}
	// crearea id-urilor pentru copiii unui nod
}

void create_ids(TArb a)
{
	if (!a) {
		return;
	}
	if (strcmp(a->info->type, "html") == 0) {
		create_ids_html_case(a);
		TArb q = a->firstChild;
		for (; q; q = q->nextSibling) {
			create_ids(q);
		}
	} else {
		TArb q = a->firstChild;
		if (q) {
			create_ids_node(a);
			q = a->firstChild;
			for (; q; q = q->nextSibling) {
				create_ids(q);
			}
		}
	}
	// crearea id-urilor la construirea arborelui
	// si la eventuale stergeri recursive
}

void print_del_error(FILE *out, char *arg, int mode) {
	if (mode == 1) {
		fprintf(out, "Delete recursively failed");
		fprintf(out, ": no node found for selector #%s!\n", arg);
	} else if (mode == 2) {
		fprintf(out, "Delete recursively failed");
		fprintf(out, ": no node found for selector %s!\n", arg);
	} else if (mode == 3) {
		fprintf(out, "Delete recursively failed");
		fprintf(out, ": no node found for selector .%s!\n", arg);
	}
	// afisarea unui mesaj de eroare in caz de esec in cautarea
	// unui element
}

void del_find_after_id (TArb a, char *sel_arg_cpy, int nr_pos, 
	int *positions, FILE *out)
{
	char found = 1;
	int i;
	TArb q = a->firstChild;
	TArb ant_q = a;
	if (!q) {
		found = 0;
		print_del_error(out, sel_arg_cpy, 1);
	}
	int cnt;
	if (found)
	for (i = 0; i < nr_pos; i++) {
		for (cnt = 1; cnt < positions[i]; cnt++) {
			ant_q = q;
			q = q->nextSibling;
			if (!q) {
				print_del_error(out, sel_arg_cpy, 1);
				found = 0;
				break;
			}
		}
		if (i != nr_pos - 1 && found) {
			ant_q = q;
			q = q->firstChild;
			if (!q) {
				print_del_error(out, sel_arg_cpy, 1);
				found = 0;
				break;
			}
		}
		if (!found) {
			break;
		}
	}
	if (found && ant_q->firstChild == q) {
		ant_q->firstChild = q->nextSibling;
	} else if (found && ant_q->nextSibling == q) {
		ant_q->nextSibling = q->nextSibling;
	}
	if (found) {
		TArb aux = q->firstChild;
		freeInfo(q->info);
		free(q);
		if (aux) {
			delete_tree_part(aux);
		}
		create_ids(a);
	}
	free(positions);
	free(sel_arg_cpy);
	// se cauta elementul dupa id
	// se sterge recursiv tot ceea ce contine subarborele cautat
	// realizandu-se in prealabil legaturile
}

void delete_el_point_el(TArb a, char *sel, int len, FILE *out)
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
	find_del_tag_class_name(a, tagName, className, &found);
	if (!found) {
		print_del_error(out, sel, 2);
	} else {
		create_ids(a);
	}
	free(tagName);
	free(className);
	free(sel_cpy);
	// stergerea tagurilor cu un anumit nume
	// si care apartin unei anumite clase
}

void delete_el_greater_el (TArb a, char *sel, int len, FILE *out)
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
	find_del_tag_parent_child(a, tagParent, tagChild, &found);
	if (!found) {
		print_del_error(out, sel, 2);
	} else {
		create_ids(a);
	}
	free(tagParent);
	free(tagChild);
	free(sel_cpy);
	// stergerea unui tag care are ca parinte un alt tag
}

void delete_el_space_el(TArb a, char *sel, int len, FILE *out)
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
	find_del_tag_ancestor_child(a, tagAncestor, tagChild, 0, &found);
	if (!found) {
		print_del_error(out, sel, 2);
	} else {
		create_ids(a);
	}
	free(tagAncestor);
	free(tagChild);
	free(sel_cpy);
	// stergerea unui element care are ca stramos alt element
}

void split_id (char *sel_arg, int **positions,
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
	// transformarea in int a numerelor stocate in ca siruri de caractere
}

void determine_between_symbol(char *symbol, char *sel, int len)
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

void deleteRecursively_tool(TArb a, char *selector, FILE *out)
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
		split_id(sel_arg, &positions, &nr_pos, &sel_arg_cpy);
		del_find_after_id(a, sel_arg_cpy, nr_pos, positions, out);
	} else if (selector[start] == '.') {
		int found; 
		find_del_tag_class(a, sel_arg, &found);
		if (!found) {
			print_del_error(out, sel_arg, 3);
		} else {
			create_ids(a);
		}
	} else {
		len = strlen(selector + start);
		char *sel = malloc(SIZE_CH * (len + 1));
		strcpy(sel, selector + start);
		sel[len - 1] = '\0';
		len--;
		char symbol = '\0';
		determine_between_symbol(&symbol, sel, len);
		if (symbol == '\0') {
			int found = 0;
			find_del_tag_name(a, sel, &found);
			if (!found) {
				print_del_error(out, sel, 2);
			} else {
				create_ids(a);
			}
		} else if (symbol == '.') {
			delete_el_point_el(a, sel, len, out);
		} else if (symbol == '>') {
			delete_el_greater_el(a, sel, len, out);
		} else if (symbol == ' ') {
			delete_el_space_el(a, sel, len, out);
		}
		free(sel);
	}
	free(sel_arg);
	// in functie de selectorul din comanda
	// se apeleaza o functie specializata
}