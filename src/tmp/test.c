#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

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



Element * trouver(char*statement){
  Element *curseur=variables.top;
  while(strcmp(curseur->nom,statement) && curseur->suivant!=0){
    curseur=curseur->suivant;
  }
  if (strcmp(curseur->nom,statement)){
      return NULL;
    }else
       return curseur;
}


void element_free(Element *e){
  if (e->suivant){
    element_free(e->suivant);
  }
  free(e);
}
void variables_free(Liste varibles){
  Element *tmp=variables.top;
  if(tmp->suivant!=NULL){
    element_free(tmp->suivant);
 }
}

void inserer(char *statement,int val){
  Element * t=trouver(statement);
  if (t!=NULL)
    t->valeur=val;
  else{
    t=malloc(sizeof(Element));
    variables.bottom->suivant=t;
    variables.bottom=t;
    t->valeur=val;
    strcpy(t->nom,statement);
    t->suivant=NULL;
  }
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 

int valueof(char*nom){
  Element *tmp=trouver(nom);
  if (tmp)
    return tmp->valeur;
  else{
    yyerror("variable non trouvé");
    exit(-1);
  } 
}


void main(void){
  variables.top=&first;
  variables.bottom=&first;
  first.suivant = NULL;
  assert(trouver("ab") == NULL);
  inserer("ab",1);
  assert(trouver("ab") != NULL);
  assert( valueof("ab") == 1 );
  inserer("ab",3);
  assert( valueof("ab") != 1 );
  assert( valueof("ab") == 3 );
  inserer("a",50);
  assert( valueof("ab") == 3 );
  assert(trouver("a") != NULL);
  assert( valueof("a") == 50 );
  variables_free(variables);
}
