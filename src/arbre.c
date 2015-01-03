#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "arbre.h"
#include "parser.tab.h"

node *mknode(node *left, node *right, int tokcode, char *token)
{
  /* malloc the node */
  node *newnode = (node *)malloc(sizeof(node));
  char *newstr = (char *)malloc(strlen(token)+1);
  strcpy(newstr, token);
  newnode->left = left;
  newnode->right = right;
  newnode->token = newstr;
  left->position=1;
  if (right->tokcode==ICONSTANT || right->tokcode==FCONSTANT)
    right->position=2;
  else 
    right->position=3;
  newnode->tokcode = tokcode;
  return(newnode);
}

void ajouter_declaration(tree *t, List_Var *l, List_Var_Global *L, int type,char bf){
  if(t->left!=NULL && t->right!=NULL){
    ajouter_declaration(t->left,type,bf);
    return ajouter_declaration(t->right,type,bf);
  }
  if (t->right==NULL)
    return ajouter_declaration(t->left,type,bf);
  if (t->left==NULL)
    return ajouter_declaration(t->right,type,bf);
  
  switch (type){

  case 1:
    if (bf=1){
      ajouter_varI(l,t->token,0,0);
      printf("sub $4,%%rsp\n");
      return;
    }
    else {
      ajouter_varGlobI(L,t->token,0);
    }
    break;

  case 2:
    if (bf=1)
      return ajouter_varF(l,t->token,0,0);
    else 
      return ajouter_varGlobF(L,t->token,0);
    break;

  case 3:
    if (bf=1)
      return ajouter_varI(l,t->token,0,1);
    else 
      return ajouter_varGlobI(L,t->token,1);
    break;

  case 4:
    if (bf=1)
      return ajouter_varF(l,t->token,0,1);
    else 
      return ajouter_varGlobF(L,t->token,0,1);
    break;
    
  }
  
}

void generate(node *tree, List_Var *l, List_Var_Global *L)
{
  int i;

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
    /* push the number onto the stack */
    if (tree->position==1)//le noeud est un fils guache du noeud pere
      printf("mov %s,%%eax\n",tree->reg);
    else //le noeud est un fils droit
      printf("mov %s,%%ebx\n",tree->reg);
    break;

  case FCONSTANT:
    /* push the number onto the stack */
    
    break;
    
  case IDENTIFIER :
    {
      Var *v;
      if (v=trouver_localement(l,tree->token)){
	if (tree->position==1)
	  printf("mov %d(%%rbp),%%eax",v->offset);  
	else 
	  printf("mov %d(%%rbp),%%ebx",v->offset);  
	char nom[100];
	sprintf(nom,"%d(%%rbp)",v->offset);
	tree->reg=strdup(v->nom);
      }
      else{
	Var_Global* V;
	if (V=trouver_var_glob(L,tree->token)){
	  if (tree->position==1)
	    printf("mov %s,%%eax",V->nom);
	  else 
	    printf("mov %s,%%ebx",V->nom);
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
    if (in_fonction==0){
      tree *f=tree->right;
      ajouter_varGlobI(L,)
    }
    break;
  

  case ADD:      
    printf("add %%ebx,%%eax\n");
    if (tree->position==3)//fils droit
      printf("movl %%eax,%%ecx\n");
    // sinon on l'enregistre dans eax mais c'est deja fait
    break;

  case AFF:
    if (tree->left->tokcode==IDENTIFIER){
      char *registre=tree->left->reg;
    }
    else
      yyerror("erreur de syntaxe");
    printf("mov %%ebx,%s\n",tree->left->reg);
    break;
  case MINUS:
   
    break;

  case TIMES:
    printf("POP A\n");
    printf("POP B\n");
    printf("MULT A= A*B\n");
    printf("PUSH A\n");
    break;

  default:
    printf("error unkown AST code %d\n", tree->tokcode);
  }

}

