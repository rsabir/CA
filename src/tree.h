#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct var_global{
  /* Var * top; */
  /* Var * bottom; */
  List_var *blocs_top;
  List_var *blocs_bottom;
  char type;
  char nom[100];
  struct var_global *next; 
} Var_Global;


typedef struct list_var_global{
  Var_Global *top;
  Var_Global *bottom;
}List_var_Global;

typedef struct var {
  char nom[100];
  int offset;
  struct var* next;
} Var;

typedef struct list_var{
  Var *top;
  Var *bottom;
  struct list_var *next;
  struct list_var *previous;
}Liste_var;


void ajouter_fonction(List_Var_Global *l,char *nom);
void ajouter_varGlobF(List_Var_Global *l,char *nom);
void ajouter_varGlobI(List_Var_Global *l,char *nom);

void ajouter_varF(List_Var *l,char *nom);
void ajouter_varI(List_Var *l,char *nom);

void ajouter_block(Var_Global *l);

/*
** Permet de trouver une variable dans la fonction et sinon dans la table des variables globales. Dans le cas d'echec retourne null
*/
Var *trouver(List_var_Glob *lg,List_Var *l,char *nom);
/*
** Fonction intermediaire utilisé par la fonction trouver pour chercher une variable global. Retourne null dans le cas d'echec
*/
Var_Global *trouver_varGlob(List_Var_Glob *l,char *nom);

Var_Global *trouver_fonction(List_Var_Glob *l,char *nom);

List_Var *pop_block(List_Var *l);
/*
** utilise que par free_fonction et free list_var_glob (ne peut pas être utilisé directement: engendre des bugs)
*/
void free_Var_Global(Var_Global* v);
/*
** nom: est le nom de la fonction a supprimer. free_fonction considere que la fonction existe dans lg!
*/
void free_fonction(List_Var_Global *lg, char *nom);

void free_List_Var_Global(List_Var_Global *lg);
