#include "tree.h"

Var_Global * ajouter_varGlob(List_Var_Global *l,char *nom){
 Var_Global * tmp=malloc(sizeof(Var_Global));
  strcpy(tmp->nom,nom);
  tmp->blocs_top=NULL;
  tmp->blocs_bottom=NULL;
  tmp->next=NULL;
  l->bottom->next=tmp;
  l->bottom=tmp; 
  return tmp;
}


void ajouter_fonction(List_Var_Global *l,char *nom){
  Var_Global * tmp=ajouter_varGlob(l,nom);
  ajouter_block(tmp);
  tmp->type=-1;
}

void ajouter_varGlobF(List_Var_Global *l,char *nom){
  Var_Global * tmp=ajouter_varGlob(l,nom);
  tmp->type=2;
}

void ajouter_varGlobI(List_Var_Global *l,char *nom){
  Var_Global * tmp=ajouter_varGlob(l,nom);
  tmp->type=1;
}

Var* ajouter_var(List_Var *l, char *nom){
  Var *tmp=malloc (sizeof(Var));
  strcpy( tmp->nom , nom );
  l->bottom->next=tmp;
  tmp->next=NULL;
  l->bottom=tmp; 
  return tmp;
}
void ajouter_varF(List_Var *l,char *nom){
  tmp=ajouter(l,nom);
  tmp->type=2;
}

void ajouter_varI(List_Var *l,char *nom){
  tmp=ajouter(l,nom);
  tmp->type=1;
}

void ajouter_block(Var_Global *l ){
  List_Var *tmp=malloc(sizeof(List_Var));
  tmp->top=NULL;
  tmp->bottom=NULL;
  if (l->bloc_bottom==NULL){ //si la fonction vient d'être construit
    l->bloc_bottom=tmp;
    l->block_top=tmp;
    tmp->next=NULL;
    tmp->previous=NULL;
  }
  else { //ajouter un block dans une fonction cela est équivalent à avoir un if ou une boucle dans la fonction
    l->block_bottom->next=tmp;
    tmp->previous=l->block_bottom;
    tmp->next=NULL;
    l->block_bottom=tmp;
  }   
}

Var *trouver(List_Var_Global *Lg,List_Var *l,char *nom){
  Var *curseur= l->top;
   while(strcmp(curseur->nom,nom) && curseur->next!=0){
     curseur=curseur->next;
   }
   if (strcmp(curseur->nom,nom)){
     if (l->previous!=null)
       trouver(lg,l->previous,nom);
     else{
       Var_Global * tmp=trouver_VarGlob(lg,nom);
       if (tmp==null)
	 return null;
       else{
	 Var *varglob=malloc(sizeof(Var));
	 varglob->next=NULL;
	 varglob->offset=0;
	 strcpy(varglob->nom,nom); 
	 return varglob;
       }	   
     }
   }else
     return curseur;
}

Var_Global *trouver_Var_Global(List_Var_Glob *l,char *nom,int type){
  Var *curseur= l->top;
  if (type!=-1){
    while((strcmp(curseur->nom,nom) || curseur->type==-1) && curseur->next!=0  ){
      // tant qu'on trouve pas une variable (et non pas une fonction [type=-1]) de meme nom
      curseur=curseur->next;
    }
  }else
    while((strcmp(curseur->nom,nom) || curseur->type!=-1) && curseur->next!=0  ){
      curseur=curseur->next;
    }

   if (strcmp(curseur->nom,nom)){
     return null;
   }
   return curseur;
}

Var_Global *trouver_varGlob(List_Var_Glob *l,char *nom){
  return trouver_Var_Global(l,nom,1);
}

Var_Global *trouver_fonction(List_Var_Glob *l,char *nom){
  return trouver_Var_Global(l,nom,-1);
}

void free_var(Var *v ){
  if (v->next!=NULL)
    free_var(v->next);
  free(v);
}

List_Var* pop_block(List_Var *l){
  if (l->var!=NULL)
    free_var(l->var);
  l->previous->next=NULL;
  return l->previous;
}

void free_Var_Global(Var_Global *v){
  List_Var *tmp=v->blocs_bottom;
  while(tmp!=null){
    tmp=pop_block(tmp);
  }
  free(v);
}

void free_fonction(List_Var_Global *lg, char *nom){
  Var_GLobal* curseur=lg->top;
  if ((strcmp(lg->top->nom,nom) || curseur->type!=-1)){
    while((strcmp(curseur->next->nom,nom) || curseur->next->type!=-1) && curseur->nom->next!=0  ){
      curseur=curseur->next;
    }
    Var_Global fonction=curseur->next;
    curseur->next=curseur->next->next;
    free_Var_Global(fonction);    
  }
  else{
    lg->top=lg->top-next;
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

