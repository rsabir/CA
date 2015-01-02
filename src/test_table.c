#include "table.h"
#include <assert.h>

void test_ajouter_fonction(void){
  List_Var_Global *l=initialiser_list_var_global();
  char *nom="lol";
  ajouter_fonction(l,nom,1);
  nom="la";
  ajouter_fonction(l,nom,2);
  assert(strcmp(l->top->nom,"lol")==0);
  assert(l->top->blocs_top->next==NULL);
  assert(l->top->blocs_bottom->previous==NULL);
  assert(l->top->blocs_bottom==l->top->blocs_top);
  assert(strcmp(l->top->next->nom,nom)==0);
  assert(l->top->next->next==NULL);
  assert(l->top->type==-1);
  assert(l->top->next->type==-1);
  free_List_Var_Global(l);
}

void test_ajouter_varGlobF(void){
  List_Var_Global *l=initialiser_list_var_global();
  ajouter_varGlobF(l,"a");
  assert(strcmp(l->top->nom,"a")==0);
  assert(l->top->type==2);
  ajouter_varGlobF(l,"b");
  assert(strcmp(l->top->next->nom,"b")==0);
  assert(l->top->next->type==2);
  free_List_Var_Global(l);
}

void test_ajouter_varGlobI(void){
  List_Var_Global *l=initialiser_list_var_global();
  ajouter_varGlobI(l,"a");
  assert(strcmp(l->top->nom,"a")==0);
  assert(l->top->type==1);
  ajouter_varGlobI(l,"b");
  assert(strcmp(l->top->next->nom,"b")==0);
  assert(l->top->next->type==1);
  free_List_Var_Global(l);
}

void test_ajouter_varF(void){
  List_Var *l=malloc(sizeof(List_Var));
  l->previous=NULL;
  l->next=NULL;
  l->top=NULL;
  l->bottom=NULL;
  ajouter_varF(l,"a",8);
  assert(strcmp(l->top->nom,"a")==0);
  assert(l->top->offset==8);
  assert(l->top->next==NULL);
  assert(l->top==l->bottom);
  ajouter_varF(l,"b",12);
  assert(strcmp(l->top->next->nom,"b")==0);
  assert(l->top->next->next==NULL);
  assert(l->bottom->offset==12);
  assert(l->top!=l->bottom);
  assert(l->bottom==l->top->next);
  free(l->top);
  free(l->bottom);
  free(l);
}

void test_ajouter_varI(void){
  List_Var_Global *l=initialiser_list_var_global();
  char *nom="lol";
  List_Var *lol_block=ajouter_fonction(l,nom,2);  
  ajouter_varI(lol_block,"a",8);
  assert(strcmp(lol_block->top->nom,"a")==0);
  assert(lol_block->top->offset==8);
  assert(lol_block->top->next==NULL);
  assert(lol_block->top==lol_block->bottom);
  ajouter_varF(lol_block,"b",12);
  assert(strcmp(lol_block->top->next->nom,"b")==0);
  assert(lol_block->top->next->next==NULL);
  assert(lol_block->bottom->offset==12);
  assert(lol_block->top!=lol_block->bottom);
  assert(lol_block->bottom==lol_block->top->next);
  free_List_Var_Global(l);  
}

void test_ajouter_block(void){
  List_Var_Global *l=initialiser_list_var_global();
  char *nom="lol";
  ajouter_fonction(l,nom,1);  
  Var_Global *lol=l->top;
  assert(lol->blocs_bottom==lol->blocs_top);
  ajouter_block(lol);
  assert(lol->blocs_bottom!=lol->blocs_top);
  List_Var *tmp=lol->blocs_bottom;
  ajouter_block(lol);
  assert(lol->blocs_bottom!=tmp);
  free_List_Var_Global(l);  
}

void test_trouver_localement(void){
  List_Var_Global *l=initialiser_list_var_global();
  char *nom="lol";
  List_Var* lol_block=ajouter_fonction(l,nom,1);  
  Var_Global *lol=l->top;
  ajouter_varF(lol_block,"a",8);
  ajouter_varI(lol_block,"b",8);
  assert(trouver_localement(lol_block,"lol")==NULL);
  assert(trouver_localement(lol_block,"a"));
  assert(trouver_localement(lol_block,"b"));
  List_Var* btmp=ajouter_block(lol);
  ajouter_varF(btmp,"ab",8);
  assert(trouver_localement(btmp,"lol")==NULL);
  assert(trouver_localement(btmp,"a"));
  assert(trouver_localement(btmp,"b"));
  assert(trouver_localement(lol_block,"a"));
  assert(trouver_localement(lol_block,"ab")==NULL);
  List_Var* btmpp=ajouter_block(lol);
  ajouter_varF(btmpp,"c",8);
  assert(trouver_localement(btmp,"lol")==NULL);
  assert(trouver_localement(btmpp,"b"));
  assert(trouver_localement(btmpp,"a"));
  assert(trouver_localement(btmpp,"ab"));
  assert(trouver_localement(btmpp,"c"));
  free_List_Var_Global(l);	 
}

void test_trouver_fonction(void){
  List_Var_Global *l=initialiser_list_var_global();
  char *nom="lol";
  ajouter_fonction(l,nom,2);  
  ajouter_fonction(l,"lafonction",2);  
  assert(trouver_fonction(l,"lafonction"));
  assert(trouver_fonction(l,"lol"));
  assert(trouver_fonction(l,"les")==NULL);
  ajouter_fonction(l,"lafonctionuno",1);  
  assert(trouver_fonction(l,"lafonction"));
  assert(trouver_fonction(l,"lol"));
  assert(trouver_fonction(l,"lafonctionuno"));
  assert(trouver_fonction(l,"les")==NULL);
  free_List_Var_Global(l);	 
}

//toutes les fonctions free ont été testés dans les fonctions precedentes

void main(void){
  test_ajouter_fonction();
  test_ajouter_varGlobF();
  test_ajouter_varGlobI();
  test_ajouter_varF();
  test_ajouter_varI();
  test_ajouter_block();
  test_trouver_localement();
  test_trouver_fonction();
}
