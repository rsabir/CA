void yyerror (char *s);
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

typedef struct element{
  char nom[100];
  int valeur;
  struct element *suivant;
}Element;
  typedef struct liste{
    struct element *top;
    struct element *bottom;
  } Liste;
Liste variables;
Element first;

void variables_free(Liste varibles);
void inserer(char *,int);
int valueof(char*);
Element * trouver(char*statement);
void element_free(Element *e);
int valueof(char*nom);
