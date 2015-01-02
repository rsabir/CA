#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"

 typedef struct node
  {
    struct node *left;
    struct node *right;
    int tokcode;
    char *token;
  } node;


node *mknode(node *left, node *right, int tokcode, char *token)
{
  /* malloc the node */
  node *newnode = (node *)malloc(sizeof(node));
  char *newstr = (char *)malloc(strlen(token)+1);
  strcpy(newstr, token);
  newnode->left = left;
  newnode->right = right;
  newnode->token = newstr;
  newnode->tokcode = tokcode;
  return(newnode);
}


void generate(node *tree)
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
    printf("PUSH %s\n", tree->token);
    break;

   case PLUS:      
    
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

