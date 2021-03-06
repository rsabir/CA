#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

extern int yylineno;
union Data{
  int i;
  float f;
};
typedef struct element{
  char nom[100];
  Data valeur;
  char type;
  struct element *suivant;
}Element;
typedef struct liste{
  struct element *top;
  struct element *bottom;
} Liste;
Liste variables;
Element first;

int yylex (); 
void yyerror (char *s);
void variables_free(Liste varibles);
void inserer(char *,int);
int valueof(char*);
Element * trouver(char*statement);
void element_free(Element *e);
int valueof(char*nom);
