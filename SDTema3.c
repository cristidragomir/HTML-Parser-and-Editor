/* Dragomir Constantin-Cristian 314CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"
#include "init_tree.h"
#include "tree_delete.h"
#include "append_override.h"

#define MAX 1000
#define SIZE_CH sizeof(char)

void action_status_1_1(TArb *arb, char *c)
{
	if (!(*arb)->info) {
		(*arb)->info = InitInfo();
	}
	TInfo prefix = (*arb)->info;
	int len = 0;
	if (prefix->contents) {
		len = strlen(prefix->contents);
	}
	char *new_str = malloc(SIZE_CH * (len + 2));
	if (!new_str) {
		return;
	}
	if (prefix->contents) {
		strcpy(new_str, prefix->contents);
		strcat(new_str, c);
		free(prefix->contents);
	} else {
		strcpy(new_str, c);
	}
	prefix->contents = new_str;
	// se completeaza campul contents din informatia nodului

}

void action_status_3_3 (TArb *arb, char *c)
{
	if (!(*arb)->info) {
		(*arb)->info = InitInfo();
	}
	TInfo prefix = (*arb)->info;
	int len = 0;
	if (prefix->type) {
		len = strlen(prefix->type);
	}
	char *new_str = malloc(SIZE_CH * (len + 2));
	if (!new_str) {
		return;
	}
	if (prefix->type) {
		strcpy(new_str, prefix->type);
		strcat(new_str, c);
		free(prefix->type);
	} else {
		strcpy(new_str, c);
	}
	prefix->type = new_str;
	// se completeaza restul numelui tagului
}

void action_status_2_3(TArb *arb, char *c)
{
	*arb = InitArb();
	if (!(*arb)->info) {
		(*arb)->info = InitInfo();
	}
	TInfo prefix = (*arb)->info;
	int len = 0;
	if (prefix->type) {
		len = strlen(prefix->type);
	}
	char *new_str = malloc(SIZE_CH * (len + 2));
	if (!new_str) {
		return;
	}
	if (prefix->type) {
		strcpy(new_str, prefix->type);
		strcat(new_str, c);
		free(prefix->type);
	} else {
		strcpy(new_str, c);
	}
	prefix->type = new_str;
	// se creeaza un nou arbore si i se atribuie primul caracter
	// din nume (in caz ca nu exista un spatiu dupa <)
}

int action_status(TArb *arb, char *c, 
	char *name, char *value,
	TParseState pr_st, TParseState act_st)
{
	if (pr_st == 1 && act_st == 1) {
		action_status_1_1(arb, c);
	} else if (pr_st == 3 && act_st == 3) {
		action_status_3_3(arb, c);
	} else if (pr_st == 2 && act_st == 3) {
		action_status_2_3(arb, c);
	} else if ((pr_st == 6 && act_st == 6) || (pr_st == 5 && act_st == 6)) {
		strcat(name, c);
	} else if (pr_st == 8 && act_st == 8) {
		strcat(value, c);
	} else if (pr_st == 8 && act_st == 5) {
		InitAttr(*arb, name, value);
	}
	return 1;
	// in functie de valoarea anterioara si curenta a automatului
	// se realizeaza o anumita operatiune pentru crearea arborelui
}

TArb get_pos_tree(TArb init, int lvl)
{
	TArb new_pos = init;
	while(lvl)
	{
		for(;new_pos->nextSibling; new_pos = new_pos->nextSibling);
		new_pos = new_pos->firstChild;
		lvl--;
	}
	for(;new_pos->nextSibling; new_pos = new_pos->nextSibling);
	return new_pos;
	// se doreste revenirea la un nivel anterior
}

void AfisareStil(TAttr p, FILE *out)
{
	if (!p) {
		return;
	} 
	fprintf(out, " style=\"");
	for (;p;p = p->next) {
		fprintf(out, "%s: ", p->name);
		fprintf(out, "%s;", p->value);
		if (p->next) {
			fprintf(out, " ");
		}
	}
	fprintf(out, "\"");
	// afisarea stilurilor
}

void AfisareAlteAtrib(TAttr p, FILE *out)
{
	if (!p) {
		return;
	}
	fprintf(out, " ");
	for (;p;p = p->next) {
		fprintf(out, "%s=", p->name);
		fprintf(out, "\"%s\"", p->value);
		if (p->next) {
			fprintf(out, " ");
		}
	}
	// afisarea celorlalte atribute		
}

void AfisareContinut(TArb q, FILE *out, int niv)
{
	int bec = 0;
	int lim, jj;
	if (!q->info->contents) {
		return;
	}
	// inexistenta continutului
	TInfo prfx = q->info;
	for (jj = 0; prfx->contents[jj] == ' ' 
		|| prfx->contents[jj] == '\n'; jj++);
	// evitarea spatiilor si a newline-urilor de la inceput
	lim = strlen(prfx->contents) - 1;
	while (lim >= 0 && (prfx->contents[lim] == ' ' 
		|| prfx->contents[lim] == '\n')) {
		lim--;
	}
	// evitarea spatiilor si a newline-urilor de la sfarsit
	for (; jj <= lim; jj++) {
		if (!bec) {
			int k;
			for (k = 1; k <= niv + 1; k++) {
				fprintf(out, "	");
			}
			bec = 1;
		}
		fprintf(out, "%c", prfx->contents[jj]);
	}
	// scrierea continutului in fisier
	if (bec) {
		fprintf(out, "\n");
	}
	// daca s-a gasit continut de scris se tasteaza un newline
}

void AfisareArb(FILE *out, TArb arb, int niv) {
	if (!arb) {
		return;
	}
	TArb q = arb;
	for(; q; q = q->nextSibling) {
		int j;
		for (j = 1; j <= niv; j++) {
			fprintf(out, "	");
		}
		// afisarea unui numar de taburi
		fprintf(out, "<%s", q->info->type);
		TAttr p = q->info->style;
		AfisareStil(p, out);
		// se afiseaza lista de atribute de stil
		p = q->info->otherAttributes;
		AfisareAlteAtrib(p, out);
		// afisarea listei cu celelalte atribute
		if (q->info->isSelfClosing) {
			fprintf(out, "/>\n");
		} else {
			fprintf(out, ">\n");
		}
		// tratarea cazului in care tagul este de tip self-enclosing
		AfisareContinut(q, out, niv);
		// afisare continut
		AfisareArb(out, q->firstChild, niv+1);
		// afisarea tagurilor interioare
		if (!q->info->isSelfClosing) {
			for (j = 1; j <= niv; j++) {
				fprintf(out, "	");
			}
			fprintf(out, "</%s>\n", q->info->type);
		}
		// incheierea tagului
	}
}

void post_processing_tree_case_1 (TAttr prefix, char **cleanStNameVal, 
	char *stNameVal, int len_2)
{
	int i;
	prefix->name = malloc(SIZE_CH * (len_2 + 1));
	*cleanStNameVal = malloc(SIZE_CH * (len_2 + 1));
	int j = 0;
	for (i = 0; i < len_2; i++) {
		if (stNameVal[i] != ' ') {
			(*cleanStNameVal)[j++] = stNameVal[i];
		}
	}
	(*cleanStNameVal)[j] = '\0';
	strcpy(prefix->name, *cleanStNameVal);
	free(*cleanStNameVal);
	// se doreste eliminarea spatiilor din numele atributului de tip style
}

void post_processing_tree_case_2 (TAttr prefix, char **cleanStNameVal, 
	char *stNameVal, int len_2)
{
	int i;
	prefix->value = malloc(SIZE_CH * (len_2 + 1));
	*cleanStNameVal = malloc(SIZE_CH * (len_2 + 1));
	int j = 0;
	for (i = 0; i < len_2; i++) {
		if (stNameVal[i] != ' ') {
			(*cleanStNameVal)[j++] = stNameVal[i];
		}
	}
	(*cleanStNameVal)[j] = '\0';
	strcpy(prefix->value, *cleanStNameVal);
	free(*cleanStNameVal);
	// se doreste eliminarea spatiilor din valoarea atributului de tip style
}

void post_processing_tree(TArb a) 
{
	if(!a) {
		return;
	}
	if (!a->info->style) {
		TArb q = a->firstChild;
		for (; q; q = q->nextSibling) {
			post_processing_tree(q);
		}
		return;
		// daca nu exista atributul style se trece la
		// urmatoarele noduri
	}
	TAttr prefix = a->info->style;
	free(prefix->name);
	// initial exista o lista cu un singur element
	// al carui nume este style
	int len = strlen(prefix->value);
	char *styleString = malloc(SIZE_CH * (len + 1));
	strcpy(styleString, prefix->value);
	free(prefix->value);
	// se face o copiere a intregului string ce contine atributele
	prefix->name = prefix->value = NULL;
	char *stNameVal = strtok(styleString, ":;");
	// in acest string se va memora numele sau valoarea unui atribut
	char *cleanStNameVal;
	char bec = 1;
	while(stNameVal) {
		int len_2 = strlen(stNameVal);
		if (bec == 1) {
			bec = 2;
			post_processing_tree_case_1(prefix, &cleanStNameVal,
				stNameVal, len_2);
			// prelucrare nume atribut de stil
		} else {
			bec = 1;
			post_processing_tree_case_2(prefix, &cleanStNameVal,
				stNameVal, len_2);
			// prelucrare valoare atribut de stil
		}
		stNameVal = strtok(NULL, ":;");
		if (bec == 1 && stNameVal) {
			InitAttrStyle(prefix);
			prefix = prefix->next;
		}
		// cautarea urmatorului nume de atribut din lista
	}
	free(styleString);
	TArb q = a->firstChild;
	for (; q; q = q->nextSibling) {
		post_processing_tree(q);
	}
	// prelucrarea urmatoarelor noduri
}

void find_add_node (char *found, TArb *q, FILE *out,
	int nr_pos, int *positions, char *id_val) 
{
	if (!(*q)) {
		*found = 0;
		fprintf(out, "Add tag failed: node with id %s not found!\n", id_val);
		return;
	}
	int cnt, i;
	for (i = 0; i < nr_pos; i++) {
		for (cnt = 1; cnt < positions[i]; cnt++) {
			*q = (*q)->nextSibling;
			if (!(*q)) {
				fprintf(out, "Add tag failed: node with id %s not found!\n", id_val);
				*found = 0;
				break;
			}
		}
		if (i != nr_pos - 1 && *found) {
			*q = (*q)->firstChild;
			if (!(*q)) {
				fprintf(out, "Add tag failed: node with id %s not found!\n", id_val);
				*found = 0;
				break;
			}
		}
		if (!(*found)) {
			break;
		}
	}
	// se realizeaza o cautare dupa id a nodului afectat
	// pentru fiecare numar dintre puncte se vor face atatea iteratii
	// prin lista de copii a nodului
	// este posibil ca acesta sa nu se gaseasca si sa se afiseza
	// un mesaj de eroare aferent
}

void gen_id_newnode (TArb *q)
{
	*q = (*q)->firstChild;
	while ((*q)->nextSibling) {
		*q = (*q)->nextSibling;
	}
	(*q)->nextSibling = InitArb();
	int len = strlen((*q)->info->id);
	char *new_id = malloc(SIZE_CH * (len + 2));
	// sirul in care se va genera noul id la adaugarea nodului
	// se tine cont de situatia in care 99->100 (2 cifre, 3 cifre)
	char *aux = (*q)->info->id + len - 1;
	while (*aux != '.') {
		aux--;
	}
	// cautarea in sirul ID a locului de unde incepe ultimul numar
	aux++;
	int last_nr = atoi(aux);
	last_nr++;
	// transformarea in int si incrementarea cu 1 al acestuia
	int last_nr_cpy = last_nr;
	int no_digits = 0;
	while (last_nr_cpy) {
		no_digits++;
		last_nr_cpy /= 10;
	}
	// retinerea numarului de cifre
	char *conv_last_nr = malloc(SIZE_CH * (no_digits + 1));
	conv_last_nr[no_digits--] = '\0';
	while (last_nr) {
		conv_last_nr[no_digits--] = (last_nr % 10) + '0';
		last_nr /= 10;
	}
	// conversia int->char 
	int j = 0;
	strcpy(new_id, (*q)->info->id);
	aux = new_id + len - 1;
	while (*aux != '.') {
		aux--;
	}
	aux++;
	// pointer prin care se modifica ultimul numar
	while (conv_last_nr[j] != '\0') {
		*aux = conv_last_nr[j]; 
		aux++;
		j++;
	}
	// modificarea efectiva a ultimului numar
	*aux = '\0';
	(*q) = (*q)->nextSibling;
	(*q)->info = InitInfo();
	(*q)->info->id = new_id;
	free(conv_last_nr);
	// refacerea legaturilor
}

void gen_id_child (TArb *q)
{
	int len = strlen((*q)->info->id);
	char *new_id = malloc(SIZE_CH * (len + 3));
	strcpy(new_id, (*q)->info->id);
	new_id[len] = '.';
	new_id[len + 1] = '1';
	new_id[len + 2] = '\0';
	(*q)->firstChild = InitArb();
	*q = (*q)->firstChild;
	(*q)->info = InitInfo();
	(*q)->info->id = new_id;
	// generarea id-ului copilului si realizarea legaturilor
}

void node_type (TArb q, char **manipString)
{
	while (**manipString != '\"') {
		(*manipString)++;
	}
	while (**manipString != '<') {
		(*manipString)++;
	}
	(*manipString)++;
	q->info->type = malloc(SIZE_CH * MAX);
	int j = 0;
	while(**manipString != ' ' && **manipString != '>') {
		q->info->type[j++] = **manipString;
		(*manipString)++;
	}
	// cautarea numelui tagului in comanda si inserarea lui in nod
	q->info->type[j] = '\0';
	char *shrinkString = malloc(SIZE_CH * (strlen(q->info->type) + 1));
	strcpy(shrinkString, q->info->type);
	free(q->info->type);
	q->info->type = shrinkString;
	// evitarea excesului de memorie alocata
}

void node_content (TArb q, char **manipString, char *tag, int len)
{
	if (**manipString == ' ') {
		while (**manipString == ' ') {
			(*manipString)++;
		}
		if (**manipString == '/') {
			q->info->isSelfClosing = 1;
		}
	}
	// verificam daca tagul este selfClosing
	char *begin_content = *manipString + 1;
	char *end_content = tag + len - 1;
	while (*begin_content == ' ') {
		begin_content++;
	}
	while (*end_content != '<') {
		end_content--;
	}
	end_content--;
	while (*end_content == ' ') {
		end_content--;
	}
	// cautarea continutului
	// evitarea spatiilor care nu sunt necesare
	if (begin_content <= end_content) {
		int len_contents = (end_content - begin_content)/SIZE_CH + 2;
		q->info->contents = malloc(SIZE_CH * len_contents);
		int j = 0;
		while(begin_content <= end_content) {
			q->info->contents[j++] = *begin_content;
			begin_content++;
		}
		q->info->contents[j] = '\0';
	}
	// inserarea continutului in nod
}

void add_tool(TArb a, char *id, char *tag, FILE *out)
{
	char *id_val;
	int i, len;
	len = strlen(id);
	id_val = malloc(SIZE_CH * (len - 2));
	char *aux;
	aux = strtok(id, "=");
	aux = strtok(NULL, "=");
	strcpy(id_val, aux);
	// formatarea argumentului care ofera ID-ul
	char *id_val_cpy = malloc(SIZE_CH * (len + 1));
	strcpy(id_val_cpy, aux);
	int nr_pos = 1;
	for (i = 0; i < len - 2; i++) {
		if (id_val[i] == '.') {
			nr_pos++;
		}
	}
	int *positions = malloc(sizeof(int) * nr_pos);
	char *nr;
	nr = strtok(id_val_cpy, ".");
	i = 0;
	while (nr) {
		positions[i++] = atoi(nr);
		nr = strtok(NULL, ".");
	}
	// conversia in int a numerelor continute in ID
	char found = 1;
	TArb q = a->firstChild;
	find_add_node(&found, &q, out, nr_pos, positions, id_val);
	// cautarea nodului in care se adauga un copil
	if (!found) {
		free(id_val);
		free(id_val_cpy);
		free(positions);
		return;
	}
	if (q->firstChild) {
		gen_id_newnode(&q);
	} else {
		gen_id_child(&q);
	}
	// generarea id-ului noului copil
	len = strlen(tag);
	char *manipString = tag;
	node_type(q, &manipString);
	node_content(q, &manipString, tag, len);
	// inserare tipului tagului si al continutului
	free(id_val);
	free(id_val_cpy);
	free(positions);
	// eliberarea unor resurse
}

int process_command(TArb a, char *command, FILE *out)
{
	char isQuoteOpen = 0;
	char **arg = malloc(sizeof(char *) * 3);
	if (!arg) {
		return 0;
	}
	// initializarea unui vector de siruri de caractere
	int i;
	for (i = 0; i < 3; i++) {
		arg[i] = malloc(SIZE_CH * MAX);
		arg[i][0] = '\0';
		if (!arg[i]) {
			int j;
			for (j = 0;j < i; j++) {
				free(arg[i]);
			}
			free(arg);
			return 0;
		}
	}
	// initializarea sirurilor
	int len = strlen(command);
	int k,j;
	k = j = 0;
	for (i = 0; i < len; i++) {
		if ((command[i] != ' ' && !isQuoteOpen) || isQuoteOpen) {
			arg[k][j++] = command[i];
			arg[k][j] = '\0';
		}
		if (command[i] == ' ' && !isQuoteOpen) {
			k++;
			j = 0;
		}
		if (command[i] == '\"' && !isQuoteOpen) {
			isQuoteOpen = 1;
		} else if (command[i] == '\"' && isQuoteOpen) {
			isQuoteOpen = 0;
		}
	}
	// stocarea caracter cu caracter a informatiilor esentiale
	if (strcmp(arg[0], "deleteRecursively") == 0) {
		deleteRecursively_tool(a, arg[1], out);
	} else if (strcmp(arg[0], "format") == 0) {
		AfisareArb(out, a, 0);
	} else if (strcmp(arg[0], "add") == 0) {
		add_tool(a, arg[1], arg[2], out);
	} else if (strcmp(arg[0], "overrideStyle") == 0) {
		style_tool(a, arg[1], arg[2], out, 1);
	} else if (strcmp(arg[0], "appendStyle") == 0) {
		style_tool(a, arg[1], arg[2], out, 2);
	}
	// accesarea unei functii specializate
	for (i = 0; i < 3; i++) {
		free(arg[i]);
	}
	free(arg);
	// eliberarea unor resurse
	return 1;
}

void releaseMem(TArb HTML_code, FILE *in, FILE *out, FILE *html_file,
	char *command, char *c, char *val, char *name)
{
	delete_tree_part(HTML_code);
	fclose(in);
	fclose(out);
	fclose(html_file);
	free(command);
	free(c);
	free(val);
	free(name);
	// se elibereaza toata memoria incarcata in program
}

void additional_actions(TParseState pr_st, TParseState act_st, 
	TArb *HTML_code, TArb *aux, TArb *aux_2,
	int *lvl, char **name, char **val)
{
	if (pr_st == 2 && act_st == 3 && !(*HTML_code)) {
		*HTML_code = *aux;
		*aux_2 = *HTML_code;
		// crearea radacinii arborelui
	} else if (pr_st == 2 && act_st == 3 && HTML_code) {
		(*lvl)++;
		if ((*aux_2)->firstChild) {
			TArb q = (*aux_2)->firstChild;
			for(; q->nextSibling; q = q->nextSibling);
			q->nextSibling = *aux;
		} else {
			(*aux_2)->firstChild = *aux;
		}
		*aux_2 = *aux;
		// adaugam un copil nodului curent asupra caruia se opereaza
	}
	if (pr_st == 4 && act_st == 1) {
		if (*lvl) {
			(*lvl)--;
		}
		*aux_2 = get_pos_tree(*HTML_code, *lvl);
		// se trece la nodul de la nivelul anterior
	}
	if (pr_st == 9 && act_st == 1) {
		(*aux_2)->info->isSelfClosing = 1;
		if (*lvl) {
			(*lvl)--;
		}
		*aux_2 = get_pos_tree(*HTML_code, *lvl);
		// se trece la nodul de la nivelul anterior
	}
	if (pr_st == 8 && act_st == 5) {
		free(*name);
		free(*val);
		*name = malloc(SIZE_CH * MAX);
		*val = malloc(SIZE_CH * MAX);
		(*name)[0] = (*val)[0] = '\0';
		// se elibereaza sirurile de caractere auxiliare
	}
}

int main(int argc, char *argv[]) 
{
	TArb HTML_code = NULL;
	int lvl = 0;
	if (argc < 4) {
		printf("Numar de argumente insuficient\n");
		return 1;
	}
	FILE *html_file = fopen(argv[1], "rt");
	if (html_file == NULL) {
		return 1;
	}
	FILE *in = fopen(argv[2], "rt");
	if (in == NULL) {
		fclose(html_file);
		return 1;
	}
	FILE *out = fopen(argv[3], "wt");
	if (out == NULL) {
		fclose(in);
		fclose(html_file);
		return 1;
	}
	// deschiderea fisierelor
	TParseState pr_st = PARSE_CONTENTS;
	TParseState act_st;
	// retinerea starii anterioare si curente
	char *c = malloc(SIZE_CH * 2);
	char *val = malloc(SIZE_CH * MAX);
	char *name = malloc(SIZE_CH * MAX);
	// retinerea unui nume, valori, argument
	val[0] = name[0] = '\0';
	TArb aux = NULL;
	TArb aux_2 = NULL;
	// retinerea unor noduri, pentru construirea corecta a arborelui
	while(fgets(c, 2, html_file)) {
		act_st = Interpret(pr_st, *c);
		action_status(&aux, c, name, val, pr_st, act_st);
		additional_actions(pr_st, act_st, &HTML_code, &aux, &aux_2,
			&lvl, &name, &val);
		pr_st = act_st;
		// in functie de caracterul din fisierul de intrare se fac
		// anumite operatiuni pe arbore
	}
	create_ids(HTML_code);
	// generarea unor id-uri pentru fiecare nod din arbore
	post_processing_tree(HTML_code);
	// initial retinem intr-un sir de caractere tot continutul
	// atributului style dat de la tastatura
	char *command = malloc(SIZE_CH * MAX);
	// comanda data de la tastatura
	command[0] = '\0';
	while(fgets(c, 2, in)) {
		if (*c != '\n' && *c != EOF) {
			strcat(command, c);
		}
		if (*c == '\n') {
			process_command(HTML_code, command, out);
			command[0] = '\0';
		}
	}
	// fiecare comanda va fi procesata intr-un fel
	process_command(HTML_code, command, out);
	command[0] = '\0';
	// mai ramane o comanda de procesat
	releaseMem(HTML_code, in, out, html_file, command, c, val, name);
	// eliberarea tuturor resurselor
	return 0;
}