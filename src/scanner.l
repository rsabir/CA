D[0-9]
L[a-zA-Z_]

%{
#include <stdio.h>
#include <string.h>
#include "parser.tab.h"
#include "arbre.h"
void count();
int comment ();
int check_type ();
int column = 0;
%}

%option yylineno
%%
"/*"		{ comment();}
"//".*          { count();}
"float"		{ count(); return(FLOAT); }
"if"		{ count(); return(IF); }
"else"		{ count(); return(ELSE); }
"int"		{ count(); return(INT); }
"return"	{ count(); return(RETURN); }
"void"		{ count(); return(VOID); }
"while"		{ count(); return(WHILE); }
"for"		{ count(); return(FOR); }
"malloc"        { count(); return(MALLOC);}
"free"          { count(); return(FREE);}              

{L}({L}|{D})*	{ count(); yylval.str=strdup(yytext); return(IDENTIFIER); }

{D}+    	{ count(); yylval.i=atoi(yytext); return(ICONSTANT); }
{D}+"."{D}*     { count(); yylval.f=atof(yytext); return(FCONSTANT);}
"++"		{ count(); return(INC_OP); }
"--"		{ count(); return(DEC_OP); }
"<="		{ count(); return(LE_OP); }
">="		{ count(); return(GE_OP); }
"=="		{ count(); return(EQ_OP); }
"!="		{ count(); return(NE_OP); }
";"		{ count(); return(';'); }
"{"      	{ count(); return('{'); }
"}"      	{ count(); return('}'); }
","		{ count(); return(','); }
":"		{ count(); return(':'); }
"="		{ count(); return('='); }
"("		{ count(); return('('); }
")"		{ count(); return(')'); }
"["     	{ count(); return('['); }
"]"     	{ count(); return(']'); }
"."		{ count(); return('.'); }
"!"		{ count(); return('!'); }
"-"		{ count(); return('-'); }
"+"		{ count(); return('+'); }
"*"		{ count(); return('*'); }
"<"		{ count(); return('<'); }
">"		{ count(); return('>'); }
"\n"            {yylineno++;column=0;};
[\t\v\n\f]	{ count(); }
.		{ ;/* ignore bad characters */ }

%%

int yywrap() {
	return 1;
}

int comment() {
	char c, c1;

loop:
	while ((c = input()) != '*' && c != 0);
	if ((c1 = input()) != '/' && c != 0) {
		unput(c1);
		goto loop;
	}
	return 0;
}




void count() {
	int i;

	for (i = 0; yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;
	}
	//ECHO;
}

