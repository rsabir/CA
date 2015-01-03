#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct var_global{
  /* Var * top; */
  /* Var * bottom; */
  struct list_var *blocs_top;
  struct list_var *blocs_bottom;
  char type;
  char type_retour;
  char nom[100];
  struct var_global *next; 
} Var_Global;


typedef struct list_Var_global{
  Var_Global *top;
  Var_Global *bottom;
}List_Var_Global;

typedef struct var {
  char nom[100];
  int offset;
  struct var* next;
  char type;
} Var;

typedef struct list_var{
  Var *top;
  Var *bottom;
  int offset;
  struct list_var *next;
  struct list_var *previous;
}List_Var;

List_Var_Global *initialiser_list_var_global(void);

List_Var *ajouter_fonction(List_Var_Global *l,char *nom,char type_retour);
void ajouter_varGlobF(List_Var_Global *l,char *nom);
void ajouter_varGlobI(List_Var_Global *l,char *nom);

void ajouter_varF(List_Var *l,char *nom,int offset);
void ajouter_varI(List_Var *l,char *nom,int offset);


List_Var * ajouter_block(Var_Global *l);

/*
** Permet de trouver une variable dans la fonction et sinon dans la table des variables globales. Dans le cas d'echec retourne null. l est le dernier block de la fonction
*/
Var *trouver_localement(List_Var *l,char *nom);

Var_Global *trouver_Var_Global(List_Var_Global *l,char *nom,int type);
Var_Global *trouver_var_glob(List_Var_Global *l,char *nom);


Var_Global *trouver_fonction(List_Var_Global *l,char *nom);

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
