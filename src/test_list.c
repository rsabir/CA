#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"

void test_list_initialise(void){
  List *l=list_initialise();
  list_free(l);
}

void test_list_ajouter(void){
  List *l=list_initialise();
  list_ajouter(l,"ma");
  assert(strcmp(l->debut->expression,"ma")==0);
  list_ajouter(l,"la");
  assert(strcmp(l->debut->suivant->expression,"la")==0);
  list_free(l);
}

void test_list_print(void){
  List *l=list_initialise();
  list_ajouter(l,"ma");
  list_ajouter(l,"la");
  list_ajouter(l,"a");
  list_ajouter(l,"d");
  list_print(l);
  list_free(l);
}

//pour les tests de free cela a déjà été fait par les fonctions precedentes

void main(void){
  //  test_list_initialise();
  //  test_list_ajouter();
  test_list_print();
}
