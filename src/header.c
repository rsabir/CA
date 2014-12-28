#include "header.h"

Element * trouver(char*statement){
  Element *curseur=variables.top;
  while(strcmp(curseur->nom,statement) && curseur->suivant!=0){
    curseur=(curseur->suivant);
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

void inserer(char *statement,float val, char type){
  Element * t=trouver(statement);
  if (t!=NULL)
    t->valeur=val;
  else{
  t=malloc(sizeof(Element));
  variables.bottom->suivant=t;
  variables.bottom=t;
  t->valeur.f=val;
  t->type=type;
  strcpy(t->nom,statement);
  t->suivant=NULL;
 }
}

void yyerror (char *s) {
    fprintf (stderr, "%s dans la ligne: %d\n ", s,yylineno);
 } 

int valueof(char*nom){
  Element *tmp=trouver(nom);
  if (tmp){
    if (tmp->type==1)
      return tmp->valeur.i;
    else if(tmp->type==2)
      return tmp->valeur.f;
    else
      yyerror("variable non trouvé");
  }
  else{
    yyerror("variable non trouvé");
  } 
  
}
