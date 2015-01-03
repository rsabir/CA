#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"
#include "parser.tab.h"

node *mknode(node *left, node *right, int tokcode, char *token,char* reg1,char* reg2)
{
  /* malloc the node */
  node *newnode = (node *)malloc(sizeof(node));
  char *newstr = (char *)malloc(strlen(token)+1);
  strcpy(newstr, token);
  newnode->left = left;
  newnode->right = right;
  newnode->token = newstr;
  newnode->reg1=reg1;
  newnode->reg2=reg2;
  left->position=1;
  if (right->tokcode==ICONSTANT || right->tokcode==FCONSTANT)
    right->position=2;
  else 
    right->position=3;
  newnode->tokcode = tokcode;
  return(newnode);
}


void generate(node *tree, List_Var *l, List_Var_Global *L)
{
  int i;

  /* generate the code for the left side */
  if (tree->left)
    generate(tree->left);
  /* generate the code for the right side */
  if (tree->right)
    generate(tree->right);

  /* generate code for this node */
  
  switch(tree->tokcode)
  {
  case 0:
    /* we need no code for this node */
    break;

  case ICONSTANT:
    /* push the number onto the stack */
    if (tree->position==1)//le noeud est un fils droit du noeud pere
      printf("mov %s,%%ebx\n",tree->reg);
    else //le noeud est un fils gauche
      printf("mov %s,%%eax\n",tree->reg);
    break;

  case FCONSTANT:
    /* push the number onto the stack */
    
    break;
    
  case IDENTIFIER:
    Var *v;
    if (v=trouver_localement(l,tree->token))
          printf("move %d(%%rbp),%%eax",v.offset);  
    else{
      Var_Global* V;
      if (V=trouver_var_global(l,tree->token))
	printf("move %s,%%eax",V->nom); 	
    }
    
    break;

   case ADD:      
     printf("add %%ebx,%%eax\n");
     if (position==3)//fils droit
       printf("movl %%eax,%%ecx");
     // sinon on l'enregistre dans eax mais c'est deja fait
    break;

  case AFF:
    
    printf("");
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

