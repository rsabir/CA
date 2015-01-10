#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"
#include "parser.tab.h" 

extern void yyerror(const char *s);

node *mknode(node *left, node *right, int tokcode, char *token)
{
  /* malloc the node */
  node *newnode = (node *)malloc(sizeof(node));
  char *newstr = (char *)malloc(strlen(token)+1);
  strcpy(newstr, token);
  newnode->left = left;
  newnode->right = right;
  newnode->token = newstr;
  if (left!=0)
    left->position=1;
  if (right!=0){
    if (right->tokcode==ICONSTANT || right->tokcode==FCONSTANT)
      right->position=2;
    else 
      right->position=3;
  }
  newnode->tokcode = tokcode;
  return(newnode);
}

void ajouter_declaration(node *t, List_Var *l, List_Var_Global *L, int type){
  
  if (t==NULL){
    return ;
  }

    ajouter_declaration(t->left,l,L,type);
    ajouter_declaration(t->right,l,L,type);

    if (t->tokcode!=COMMA){
      switch (type){

      case 1:
	if (in_fonction==1){
	  ajouter_varI(l,t->token,0,0);
	  printf("sub $4,%%rsp\n");
	  return;
	}
	else {
	  ajouter_varGlobI(L,t->token,0);
	}
	break;

      case 2:
	if (in_fonction==1){
	  ajouter_varF(l,t->token,0,0);
	  return;
	}
	else {
	  ajouter_varGlobF(L,t->token,0);
	  return ;
	}
	break;

      case 3:
	if (in_fonction==1){
	  ajouter_varI(l,t->token,0,1);
	  return;
	}
	else {
	  ajouter_varGlobI(L,t->token,1);
	  return;
	}
	break;

      case 4:
	if (in_fonction==1){
	  ajouter_varF(l,t->token,0,1);
	  return;
	}
	else {
	  ajouter_varGlobF(L,t->token,1);
	}
	break;    
      }
  }
  
}

void generate(node *tree, List_Var *l, List_Var_Global *L)
{
  //int i;

  /* generate the code for the left side */
  if (tree->left)
    generate(tree->left,l,L);
  /* generate the code for the right side */
  if (tree->right)
    generate(tree->right,l,L);

  /* generate code for this node */
  
  switch(tree->tokcode){
  
  case 0:
    /* we need no code for this node */
    break;
    
  case ICONSTANT:
    tree->reg=malloc(100*sizeof(char));
    sprintf(tree->reg,"$%s",tree->token);
    if (tree->position==1)//le noeud est un fils guache du noeud pere
      printf("movl %s,%%eax\n",tree->reg);
    else //le noeud est un fils droit
      printf("movl %s,%%ebx\n",tree->reg);
    break;

  case FCONSTANT:
    /* push the number onto the stack */
    
    break;
    
  case IDENTIFIER :
    {
      Var *v;
      if ( l!=NULL && ((v=trouver_localement(l,tree->token))!=NULL)){
	if (tree->position==1)
	  printf("movl %d(%%rbp),%%eax\n",v->offset);  
	else 
	  printf("movl %d(%%rbp),%%ebx\n",v->offset);  
	char nom[100];
	sprintf(nom,"%d(%%rbp)",v->offset);
	tree->reg=strdup(v->nom);
      }
      else{
	Var_Global* V;
	if ((V=trouver_var_glob(L,tree->token))!=NULL){
	  if (tree->position==1)
	    printf("movl %s,%%eax\n",V->nom);
	  else 
	    printf("movl %s,%%ebx\n",V->nom);
	  tree->reg=strdup(V->nom);
	}
	else{
	  char message[100]="Erreur: Variable ";
	  strcat(message,tree->token);
	  strcat(message," non trouvé");
	  yyerror(message);
	}
      }
    
      break;
    }
  case INT:
    {
      node *f=tree->right;
	ajouter_declaration(f,l,L,1);
      break;
    }
    
  case DECLARATOR:
    break;

  case ADD:      
    if(tree->right->position==3)
      printf("add %%ecx,%%eax\n");
    else
      printf("add %%ebx,%%eax\n");
    if (tree->position==3)//fils droit
      printf("movl %%eax,%%ecx\n");
    // sinon on l'enregistre dans eax mais c'est deja fait
    break;

  case TIMES:

    if(tree->right->position==3)
      printf("imul %%ecx,%%eax\n");
    else
      printf("imul %%ebx,%%eax\n");
    if (tree->position==3)//fils droit
      printf("movl %%eax,%%ecx\n");
    break;

  case AFF:
    if (tree->left->tokcode!=IDENTIFIER)//{
    /*   char *registre=tree->left->reg; */
    /*   int type = tree->left->tokcode; */
    /*   ajouter_declaration(tree->left, l, L, type); */
    /* } */
    /* else */
      yyerror("erreur d'affectation"); 
    if (tree->right->position==3)
      printf("mov %%ecx,%s\n",tree->left->reg);
    else
      printf("mov %%ebx,%s\n",tree->left->reg);
    break;


  case SUB:
    printf("subl %%ebx,%%eax\n");
    if (tree->position==3)//fils droit
      printf("movl %%eax,%%ecx\n");    
    break;
    
  case COMMA:
    break;
  default:
    printf("error unkown AST code %d\n", tree->tokcode);
  }

}

