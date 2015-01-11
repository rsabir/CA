#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

List * list_initialise(void){
  List *l=malloc(sizeof(List));
  l->debut=NULL;
  l->fin=NULL;
  return l;
}

void list_ajouter(List *l,char *e){
  List_element *element=malloc(sizeof(List_element));
  strcpy(element->expression,e);
  if (l->debut==NULL){
    l->debut=element;
    l->fin=element;
  }else{
    l->fin->suivant=element;
    l->fin=element;
  }
  element->suivant=NULL;
}

void list_element_execution(List_element *l, void (*fonction)(List_element*)){
  if (l->suivant!=NULL)
    list_element_execution(l->suivant,fonction);
  (*fonction)(l);
}

void printf_element(List_element *e){
  printf("%s\n",e->expression);
}

void free_element(List_element *e){
  free(e);
}

void list_print(List *l){
  if (l!=NULL)
    list_element_execution(l->debut,printf_element);
}


void list_free(List *l){
  if (l!=NULL){
    if (l->debut!=NULL)
      list_element_execution(l->debut,free_element);
    free(l);
  }
}

