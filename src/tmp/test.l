%{
#include "y.tab.h"
char * nom_variable(char *);
%}
%%

("print"|"p") {return Print ;}
"exit" { return Exit_command;}
[a-zA-Z][a-zA-Z0-9_]* {yylval.statement=strdup(yytext); return Identifier;}
[0-9]{1,} {yylval.value=atoi(yytext); return Valeur;} 
[+*-/;=] { return yytext[0]; }
"\n" {yylineno++;}
[ \t] ;
.  {yyerror("Erreur de syntaxte");}
%%

int yywrap (void) {return 1;}
