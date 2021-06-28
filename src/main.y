%{
    #include <stdio.h>
    #include <stdlib.h>

    extern int yylex();
    extern int yyparse();

    void yyerror(const char*);
    int yywrap(void);
%}

%token NUMBER

%%

lines: line lines 
     | %empty
     ;

line: expr '\n'         { printf("= %i\n", $1); }

expr: expr '+' factor   { $$ = $1 + $3; }
    | expr '-' factor   { $$ = $1 - $3; }
    | factor            { $$ = $1; }
    ;

factor: factor '*' term { $$ = $1 * $3; }
      | factor '/' term { $$ = $1 / $3; }
      | term            { $$ = $1; }
      ;

term: '(' expr ')'      { $$ = $2; }
    | NUMBER            { $$ = $1; }
    ;

%%

int main(void){
    return yyparse();
}


void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}

int yywrap() {
  return 1;
}

