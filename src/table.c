#include "table.h"

extern char in_fonction;

List_Var_Global *initialiser_list_var_global(void){
  List_Var_Global *l=malloc(sizeof(List_Var_Global));
  l->top=NULL;
  l->bottom=NULL;
  return l;
}


Var_Global * ajouter_varGlob(List_Var_Global *l,char *nom){
 Var_Global * tmp=malloc(sizeof(Var_Global));
  strcpy(tmp->nom,nom);
  tmp->blocs_top=NULL;
  tmp->blocs_bottom=NULL;
  tmp->next=NULL;
  if (l->bottom!=NULL)  
    l->bottom->next=tmp;
  else
    l->top=tmp;
  l->bottom=tmp; 
  return tmp;
}


List_Var *ajouter_fonction(List_Var_Global *l,char *nom,char type_retour){
  Var_Global * tmp=ajouter_varGlob(l,nom);
  ajouter_block(tmp);
  tmp->type=-1;
  tmp->type_retour=type_retour;
  return tmp->blocs_top;
}

void ajouter_varGlobF(List_Var_Global *l,char *nom,char pointeur){
  Var_Global * tmp=ajouter_varGlob(l,nom);
  if (pointeur==0)
    tmp->type=2;
  else 
    tmp->type=4;
  tmp->type_retour=-1;
}

void ajouter_varGlobI(List_Var_Global *l,char *nom,char pointeur){
  Var_Global * tmp=ajouter_varGlob(l,nom);
  if (pointeur==0)
    tmp->type=1;
  else
    tmp->type=3;
  tmp->type_retour=-1;
}

Var* ajouter_var(List_Var *l, char *nom,int offset){
  Var *tmp=malloc (sizeof(Var));
  strcpy( tmp->nom , nom );
  if (l->bottom!=NULL)
    l->bottom->next=tmp;
  else
    l->top=tmp;
  tmp->offset=offset;
  tmp->next=NULL;
  l->bottom=tmp; 
  return tmp;
}
void ajouter_varF(List_Var *l,char *nom,int offset,char pointeur){
  if (offset==0){
    if (pointeur==0)
      l->offset-=4;
    else
      l->offset-=8;
    Var *tmp=ajouter_var(l,nom,l->offset);
    if (pointeur==0)
      tmp->type=2;
    else
      tmp->type=4;
  }
  else{
    Var *tmp=ajouter_var(l,nom,offset);
    if (pointeur==0)
      tmp->type=2;
    else
      tmp->type=4;
  }
 
}

void ajouter_varI(List_Var *l,char *nom,int offset,char pointeur){
  if (offset==0){
    if (pointeur==0)
      l->offset-=4;
    else
      l->offset-=8;
    Var *tmp=ajouter_var(l,nom,l->offset);
     if (pointeur==0)
      tmp->type=1;
    else
      tmp->type=3;
  }
  else{
    Var *tmp=ajouter_var(l,nom,offset);
    if (pointeur==0)
      tmp->type=1;
    else
      tmp->type=3;
  }
}

List_Var * ajouter_block(Var_Global *l ){
  List_Var *tmp=malloc(sizeof(List_Var));
  tmp->top=NULL;
  tmp->bottom=NULL;
  if (l->blocs_bottom==NULL){ //si la fonction vient d'être construit
    l->blocs_bottom=tmp;
    l->blocs_top=tmp;
    tmp->next=NULL;
    tmp->previous=NULL;
  }
  else { //ajouter un block dans une fonction cela est équivalent à avoir un if ou une boucle dans la fonction
    l->blocs_bottom->next=tmp;
    tmp->previous=l->blocs_bottom;
    tmp->next=NULL;
    l->blocs_bottom=tmp;
  }   
  return tmp;
}

Var *trouver_localement(List_Var *l,char *nom){
  Var *curseur= l->top;
  while(strcmp(curseur->nom,nom) && curseur->next!=0){
    curseur=curseur->next;
  }
  if (strcmp(curseur->nom,nom)){
    if (l->previous!=NULL)
      return trouver_localement(l->previous,nom);
    else 
      return NULL; //a verifier
  }else
    return curseur;
}

Var_Global *trouver_Var_Global(List_Var_Global *l,char *nom,int type){
  Var_Global *curseur= l->top;
  if (type!=-1){
    while((strcmp(curseur->nom,nom) || curseur->type==-1) && curseur->next!=0  ){
      // tant qu'on trouve pas une variable (et non pas une fonction [type=-1]) de meme nom
      curseur=curseur->next;
    }

   if (strcmp(curseur->nom,nom) ||  curseur->type==-1){
     return NULL;
   }
  }else{
    while((strcmp(curseur->nom,nom) || curseur->type!=-1) && curseur->next!=0  ){
      curseur=curseur->next;
    }
    if (strcmp(curseur->nom,nom) ||  curseur->type!=-1){
      return NULL;
    }
  }
   return curseur;
}

Var_Global *trouver_var_glob(List_Var_Global *l,char *nom){
  return trouver_Var_Global(l,nom,1);
}

Var_Global *trouver_fonction(List_Var_Global *l,char *nom){
  return trouver_Var_Global(l,nom,-1);
}

void free_var(Var *v ){
  if (v->next!=NULL)
    free_var(v->next);
  free(v);
}

List_Var* pop_block(List_Var *l){
  if (l->top!=NULL)
    free_var(l->top);
  if (l->previous!=NULL){
    l->previous->next=NULL;
    List_Var *tmp=l->previous;
    free(l);
    return tmp ;
  }
  else {
    free(l);
    return NULL;
  }
}

void free_Var_Global(Var_Global *v){
  List_Var *tmp=v->blocs_bottom;
  while(tmp!=NULL){
    tmp=pop_block(tmp);
  }
  free(v);
}

void free_fonction(List_Var_Global *lg, char *nom){
  Var_Global* curseur=lg->top;
  if ((strcmp(lg->top->nom,nom) || curseur->type!=-1)){
    while((strcmp(curseur->next->nom,nom) || curseur->next->type!=-1) && curseur->next->next!=0  ){
      curseur=curseur->next;
    }
    Var_Global * fonction=curseur->next;
    curseur->next=curseur->next->next;
    free_Var_Global(fonction);    
  }
  else{
    lg->top=lg->top->next;
    if (lg->bottom==curseur)
      lg->bottom=NULL;
    free_Var_Global(curseur);    
  }
}

void  free_List_Var_Global_rec(Var_Global *v){
  if (v->next!=NULL)
    free_List_Var_Global_rec(v->next);
  free_Var_Global(v);
}
void free_List_Var_Global(List_Var_Global *lg){
  free_List_Var_Global_rec(lg->top);
  lg->top=NULL;
  lg->bottom=NULL;
  free(lg);
}

