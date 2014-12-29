%{
  #include "test.h"
  extern int yylineno;
%}
%union{
  char *statement;
  int value;
}
%token <statement> Identifier
%token <value> Valeur
%token <statement> Print Exit_command
%type <value> Expression Start Term ExpressionS 
%start Start

%%

Start: Start ExpressionS ';' ;
| ExpressionS ';' ;
;

ExpressionS : Expression 
| Identifier '=' Expression  { inserer($1,$3); free($1);}
| Print Expression  { printf("%d\n",$2); }
| Exit_command   { YYERROR ;}
;

Expression:   Expression '+' Term { $$=$1+$3;}
| Expression '-' Term  { $$=$1-$3; }
| Expression '*' Term  { $$=$1*$3; }
| Expression '/' Term  { $$=$1/$3; }
| Term { $$=$1; }
;

Term: Valeur { $$=$1; }
| Identifier {$$=valueof($1);};

%%

Element * trouver(char*statement){
  Element *curseur=variables.top;
  while(strcmp(curseur->nom,statement) && curseur->suivant!=0){
  curseur=(curseur->suivant);
 }
  if (strcmp(curseur->nom,statement)){
  return NULL;
 }else
    return curseur;
 }


void element_free(Element *e){
  if (e->suivant){
    element_free(e->suivant);
  }
  free(e);
}
void variables_free(Liste varibles){
  Element *tmp=variables.top;
  if(tmp->suivant!=NULL){
    element_free(tmp->suivant);
  }
}

 void inserer(char *statement,int val){
  Element * t=trouver(statement);
  if (t!=NULL)
    t->valeur=val;
  else{
  t=malloc(sizeof(Element));
  variables.bottom->suivant=t;
  variables.bottom=t;
  t->valeur=val;
  strcpy(t->nom,statement);
  t->suivant=NULL;
 }
}

void yyerror (char *s) {
    fprintf (stderr, "%s dans la ligne: %d\n ", s,yylineno);
 } 

int valueof(char*nom){
  Element *tmp=trouver(nom);
  if (tmp)
    return tmp->valeur;
  else{
    yyerror("variable non trouvé");
    exit(-1);
  } 
  
}


void main(void){
  variables.top=&first;
  variables.bottom=&first;
  first.suivant=NULL;
  yyparse();
  variables_free(variables);
}