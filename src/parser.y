%{
  #include "table.h"
  #define ADD '+'
  #define TIMES '*'
  #define SUB '-'
  #define AFF '='
  char in_fonction=0;
%}

%token <str> IDENTIFIER 
%token <i> ICONSTANT 
%token <f> FCONSTANT
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token INT FLOAT VOID
%token IDENTIFIERD
%token IF ELSE WHILE RETURN FOR MALLOC FREE
%token DECLARATOR 
%union {
  char *str;
  float f;
  int i;
}
%start program
%%

primary_expression
: IDENTIFIER { $$ = $1}
| ICONSTANT {$$ = mknode(0,0, ICONSTANT, yylval.i);}
| FCONSTANT {$$ = mknode(0,0, FCONSTANT, yylval.f);}
| '(' expression ')'
| IDENTIFIER '(' ')'
| IDENTIFIER '(' argument_expression_list ')'
| IDENTIFIER INC_OP
| IDENTIFIER DEC_OP
| IDENTIFIER '[' expression ']'
;

argument_expression_list
: expression
| argument_expression_list ',' expression
;

unary_expression
: primary_expression
| '-' unary_expression 
| '!' unary_expression
;

multiplicative_expression
: unary_expression
     | multiplicative_expression '*' unary_expression  {$$=mknode($1, $3, TIMES, "*")}; 
;

additive_expression
: multiplicative_expression
| additive_expression '+' multiplicative_expression  $$ = mknode($1, $3, ADD, "+"); 
| additive_expression '-' multiplicative_expression  $$ = mknode($1, $3, SUB, "-");
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
: IDENTIFIER '=' comparison_expression {mknod($1,$3,AFF,"=");}
| IDENTIFIER '[' expression ']' '=' comparison_expression
| comparison_expression
;

declaration
: type_name declarator_list ';'
;

declarator_list
: declarator
| declarator_list ',' declarator
;

type_name
: VOID {mknod(0,0,VOID,"void");}
| INT  {mknod(0,0,INT,"int");}
| FLOAT {mknod(0,0,FLOAT,"FLOAT");}
;

declarator
: IDENTIFIER {mknod(0,0,DECLARATOR,yylval.str);}  
| '*' IDENTIFIER
| IDENTIFIER '[' ICONSTANT ']'
| declarator '(' parameter_list ')'
| declarator '(' ')'
;

parameter_list
: parameter_declaration
| parameter_list ',' parameter_declaration
;

parameter_declaration
  : type_name declarator {$1->left=0; $1->right=$2;}
;

statement
: compound_statement
| expression_statement 
| selection_statement
| iteration_statement
| jump_statement
;

compound_statement
: '{' '}'
| '{' statement_list '}'
| '{' declaration_list statement_list '}'
;

declaration_list
: declaration
| declaration_list declaration
;

statement_list
: statement
| statement_list statement
;

expression_statement
: ';'
| expression ';'
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
| declaration
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
void yyerror (char *s) {
    fflush (stdout);
    fprintf (stderr, "%s:%d:%d: %s\n", file_name, yylineno, column, s);
}



int main (int argc, char *argv[]) {
    FILE *input = NULL;
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
	return 1;
    }
    return 0;
}
