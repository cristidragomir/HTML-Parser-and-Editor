/* Dragomir Constantin-Cristian 314CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define SIZE_CH sizeof(char)

void freeAttr(TAttr attrNode);
void freeInfo(TInfo infoNode);
void delete_tree_part(TArb a);
void find_del_tag_class(TArb a, char *className, int *found);
void find_del_tag_name(TArb a, char *tagName, int *found);
void find_del_tag_class_name(TArb a, char *tagName, 
	char *className, int *found);
void find_del_tag_parent_child(TArb a, char *tagParent, 
	char *tagChild, int *found);
void find_del_tag_ancestor_child(TArb a, char *tagAncestor, char *tagChild,
	int ancestorFound, int *found);
void deleteRecursively_tool(TArb a, char *selector, FILE *out);
void create_ids(TArb a);