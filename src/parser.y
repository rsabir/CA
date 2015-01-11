%{
#include <stdio.h>
#include "list.h"
#include "arbre.h"

  char in_fonction=0;
  List_Var_Global *L=NULL; //liste des variables globales
  List_Var *l=NULL; //liste des variables locales
  List *le=NULL;  //liste contenant du codes assembleur des variables globales
  
  int yylex();
  void yyerror(const char *s);
  
  
%}

   %token <str> IDENTIFIER 
   %token <i> ICONSTANT 
   %token <f> FCONSTANT
   %token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
   %token INT FLOAT VOID INTE VOIDE FLOATE
   %token IF ELSE WHILE RETURN FOR MALLOC FREE
   %token DECLARATOR 
   %type <t> primary_expression multiplicative_expression unary_expression additive_expression comparison_expression type_name  program external_declaration
 function_definition compound_statement declaration  declarator  declarator_list parameter_list parameter_declaration statement_list expression expression_statement

 
%union {
  char *str;
  float f;
  int i;
  struct node *t; 
 }
   %start program
%%

primary_expression
: IDENTIFIER { $$ = mknode(0,0, IDENTIFIER, yylval.str);}
| ICONSTANT {char nom[100]={0,}; sprintf(nom,"%d",yyval.i); $$ = mknode(0,0, ICONSTANT, nom);}
| FCONSTANT { char nom[100]={0,}; sprintf(nom,"%d",yyval.i); $$ = mknode(0,0, FCONSTANT, nom);}
| '(' expression ')'  { $$ = mknode(0,0, 333, yylval.str);} //faux
| IDENTIFIER '(' ')' { $$ = mknode(0,0, 333, yylval.str);} //faux
| IDENTIFIER '(' argument_expression_list ')'  { $$ = mknode(0,0, 333, yylval.str);} //faux
| IDENTIFIER INC_OP  { $$ = mknode(0,0, 333, yylval.str);} //faux
| IDENTIFIER DEC_OP  { $$ = mknode(0,0, 333, yylval.str); }//faux 
| IDENTIFIER '[' expression ']'  { $$ = mknode(0,0, 333, yylval.str);} //faux
;

argument_expression_list
: expression
| argument_expression_list ',' expression
;

unary_expression
: primary_expression {$$=$1;}
| '-' unary_expression { $$=$2;} //Faux
| '!' unary_expression { $$=$2;} //Faux
;

multiplicative_expression
: unary_expression {$$=$1;}
| multiplicative_expression '*' unary_expression  {$$=mknode($1, $3, TIMES, "*");} 
;

additive_expression
: multiplicative_expression
| additive_expression '+' multiplicative_expression  {$$ = mknode($1, $3, ADD, "+"); }
| additive_expression '-' multiplicative_expression  {$$ = mknode($1, $3, SUB, "-");}
;

comparison_expression
: additive_expression
| additive_expression '<' additive_expression
| additive_expression '>' additive_expression
| additive_expression LE_OP additive_expression
| additive_expression GE_OP additive_expression
| additive_expression EQ_OP additive_expression
| additive_expression NE_OP additive_expression
;

expression
: IDENTIFIER '=' comparison_expression {
  node *n=mknode(0,0,IDENTIFIER,$1); 
  $$=mknode(n,$3,AFF,"=");
}
| IDENTIFIER '[' expression ']' '=' comparison_expression {$$=$3;}//faux
| comparison_expression
;

declaration
: type_name declarator_list ';' { $1->left=0; $1->right=$2; $$=$1;}
;

declarator_list
: declarator
| declarator_list ',' declarator  {$$=mknode($1,$3,COMMA,",");}
;

type_name
: VOID {$$=mknode(0,0,VOID,"void");}
| VOIDE {$$=mknode(0,0,VOIDE,"void *");}
| INT  {$$=mknode(0,0,INT,"int");}
| INTE  {$$=mknode(0,0,INTE,"int *");}
| FLOAT {$$=mknode(0,0,FLOAT,"FLOAT");}
| FLOATE {$$=mknode(0,0,FLOATE,"FLOAT*");}
;

declarator
: IDENTIFIER {$$=mknode(0,0,DECLARATOR,yylval.str);}  
| '*' IDENTIFIER {
  char name[100]={};
  sprintf(name,"*%s",yylval.str);
  $$=mknode(0,0,DECLARATOR,name);
  }  
| IDENTIFIER '[' ICONSTANT ']'  {$$=mknode(0,0,33,yylval.str);} //faux
| declarator '(' parameter_list ')'  {$$=mknode(0,0,33,yylval.str);} //faux
| declarator '(' ')'  {$$=mknode(0,0,33,yylval.str);} //faux
;

parameter_list
: parameter_declaration
| parameter_list ',' parameter_declaration 
;

parameter_declaration
: type_name declarator
;

statement
: compound_statement
| expression_statement 
| selection_statement
| iteration_statement
| jump_statement
;

compound_statement
: '{' '}' {$$=NULL;} //faux
| '{' statement_list '}' {$$=$2;} //faux
| '{' declaration_list statement_list '}' {$$=NULL;} //faux
;

declaration_list
: declaration
| declaration_list declaration
;

statement_list
: statement {$$=NULL;}
| statement_list statement  {$$=NULL;}
;

expression_statement
: ';' {$$=NULL;}
| expression ';' {generate($1,l,L);}
;


selection_statement
: IF '(' expression ')' statement
| IF '(' expression ')' statement ELSE statement
;

iteration_statement
: WHILE '(' expression ')' statement
| FOR '(' expression_statement expression_statement expression ')' statement
;

jump_statement
: RETURN ';'
| RETURN expression ';'
;

program
: external_declaration
| program external_declaration
;

external_declaration
: function_definition
| declaration {generate($1,l,L);}
| expression_statement
;

function_definition
: type_name declarator compound_statement
;

%%

extern char yytext[];
extern int column;
extern int yylineno;
extern FILE *yyin;

char *file_name = NULL;

void trouver (char *nom){

}
void yyerror (const char *s) {
    fflush (stdout);
    fprintf (stderr, "%s:%d:%d: %s\n", file_name, yylineno, column, s);
    exit(1);
}


int main (int argc, char *argv[]) {
    FILE *input = NULL;
    L=initialiser_list_var_global();
    l=NULL;
    le=list_initialise();
    if (argc==2) {
	input = fopen (argv[1], "r");
	file_name = strdup (argv[1]);
	if (input) {
	    yyin = input;
	    yyparse();
	}
	else {
	  fprintf (stderr, "%s: Could not open %s\n", *argv, argv[1]);
	    return 1;
	}
	free(file_name);
    }
    else {
	fprintf (stderr, "%s: error: no input file\n", *argv);
	list_free(le);
	return 1;
    }
    list_print(le);
    list_free(le);
    return 0;
}
