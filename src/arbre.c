#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"
#include "list.h"
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
  
  if (t==NULL || t->tokcode==POINTERNB){
    return ;
  }

    ajouter_declaration(t->left,l,L,type);
    ajouter_declaration(t->right,l,L,type);

    if (t->tokcode!=COMMA){
      if (t->tokcode==POINTER){
	if (type==1) type=3;
	if (type==2) type=4;
      }
      switch (type){
	
      case 1:
	if (in_fonction==1){
	  ajouter_varI(l,t->token,0,0);
	  printf("sub $4,%%rsp\n");
	}
	else {
	  ajouter_varGlobI(L,t->token,0);
	  char expression[100];
	  sprintf(expression,"%s:   .value   0",t->token);
	  list_ajouter(le,expression);
	}
	return;
	break;
      
      case 2:
	if (in_fonction==1)
	  ajouter_varF(l,t->token,0,0);
	else 
	  ajouter_varGlobF(L,t->token,0);
	return;
	break;
	
      case 3:
	if (in_fonction==1){
	  if (t->tokcode==POINTER){
	    if (t->right->tokcode!=POINTERNB){
	      yyerror("erreur de syntax l");
	    }
	    char expression[100];
	    int of=atoi(t->right->token);
	    sprintf(expression,"  .comm  %s,%d",t->token,(of-1)*4);
	    ajouter_varI(l,t->token,0,of);
	    list_ajouter(le,expression);
	  }
	  else 
	    yyerror("erreur codage pointeur int");
	}else {
	  if (t->tokcode==POINTER){
	    if (t->right->tokcode!=POINTERNB){
	      yyerror("erreur de syntax l");
	    }
	    char expression[100];
	    int of=atoi(t->right->token);
	    sprintf(expression,"  .comm  %s,%d",t->token,(of-1)*4);
	    ajouter_varGlobI(L,t->token,of);
	    list_ajouter(le,expression);
	  }
	  else
	    yyerror("erreur codage pointeur int");
	}
	return;
	break;

    case 4:
      if (in_fonction==1){
	if (t->tokcode==POINTER){
	  if (t->right->tokcode!=POINTERNB){
	    yyerror("erreur de syntax");
	  }
	  char expression[100];
	  int of=atoi(t->right->token);
	  sprintf(expression,"  .comm  %s,%d",t->token,(of-1)*4);
	  ajouter_varF(l,t->token,0,of);
	  list_ajouter(le,expression);
	}
	else 
	  yyerror("erreur codage pointeur float");
      }
      else {
	  if (t->tokcode==POINTER){
	    if (t->right->tokcode!=POINTERNB){
	      yyerror("erreur de syntax");
	    }
	    char expression[100];
	    int of=atoi(t->right->token);
	    sprintf(expression,"  .comm  %s,%d",t->token,(of-1)*4);
	    ajouter_varGlobF(L,t->token,of);
	    list_ajouter(le,expression);
	  }
	else 
	  yyerror("erreur codage pointeur float");
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

  case POINTER:
    break;
  case POINTERNB:
    break;

  default:
    printf("error unkown AST code %d\n", tree->tokcode);
  }

}

