/* Dragomir Constantin-Cristian 314CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define SIZE_CH sizeof(char)

void modify_tag_style(TArb q, char *style, int mode);
void find_tag_class(TArb a, char *className, char *style,
	int *found, int mode);
void find_tag_name(TArb a, char *tagName, char *style, int *found, int mode);
void find_tag_class_name(TArb a, char *tagName, 
	char *className, char *style, int *found, int mode);
void find_tag_parent_child(TArb a, char *tagParent,
	char *tagChild, char *style, int *found, int mode);
void find_tag_ancestor_child(TArb a, char *tagAncestor,
	char *tagChild, int ancestorFound, char *style, int *found, int mode);
void style_tool(TArb a, char *selector, char *style, FILE *out, int mode);