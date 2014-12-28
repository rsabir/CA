yacc -d test.y
lex test.l
gcc -g lex.yy.c y.tab.c -o calc
