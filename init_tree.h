/* Dragomir Constantin-Cristian 314CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define SIZE_CH sizeof(char)

TInfo InitInfo();
TArb InitArb();
int InitAttrStyle(TAttr a);
int InitAttr(TArb a, char *name, char *value);