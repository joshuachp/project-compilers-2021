%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    extern int yylex();
    extern int yyparse();

    void yyerror(const char*);
    int yywrap(void);
%}

%union {
    int integer;
    bool boolean;
}

%token NUMBER

%token PLUS
%token MIN
%token MUL
%token DIV

%token LS
%token GR
%token LEQ
%token GEQ
%token EQ
%token NEQ
%token NOT

%token TRUE
%token FALSE

%token L_B
%token R_B

%token NL

%type<integer> NUMBER
%type<boolean> TRUE FALSE


%type<integer> math_expr expr
%type<boolean> bool_expr

%left PLUS MIN
%right MUL DIV

%left LS GR LEQ GEQ EQ NEQ
%right NOT

%%

lines: lines line
     | %empty
     ;

line: math_expr NL { printf("= %d\n", $1); }
    | bool_expr NL { printf("= %s\n", $1 ? "True" : "False"); }
    ;

expr: math_expr { $$ = $1; }
    | bool_expr { $$ = $1; }
    ;

math_expr: math_expr PLUS math_expr { $$ = $1 + $3; }
         | math_expr MIN math_expr  { $$ = $1 - $3; }
         | math_expr MUL math_expr  { $$ = $1 * $3; }
         | math_expr DIV math_expr  { $$ = $1 / $3; }
         | L_B math_expr R_B        { $$ = $2; }
         | NUMBER                   { $$ = $1; }
         ;

bool_expr: expr LS expr         { $$ = $1 < $3; }
         | expr GR expr         { $$ = $1 > $3; }
         | expr LEQ expr        { $$ = $1 <= $3; }
         | expr GEQ expr        { $$ = $1 >= $3; }
         | expr EQ expr         { $$ = $1 == $3; }
         | expr NEQ expr        { $$ = $1 != $3; }
         | NOT expr             { $$ = !$2; }
         | L_B bool_expr R_B    { $$ = $2; }
         | TRUE                 { $$ = $1; }
         | FALSE                { $$ = $1; }
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

