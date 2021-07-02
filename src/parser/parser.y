%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include "tree.h"

    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;


    void yyerror(Program* program, const char*);
    int yywrap(void);
%}

%code requires {
    #include <stdbool.h>
    #include <stdint.h>
    #include "tree.h"
}

%define parse.error verbose

%parse-param {Program *program}

%union {
    char* id;
    int32_t value;
    struct Node *node;
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

%token IF
%token THEN
%token DO
%token ELSE

%token ID
%token ASSIGN

%type<id> ID
%type<value> NUMBER TRUE FALSE

%type<node> line math_expr expr cond bool_expr var

%left PLUS MIN
%right MUL DIV

%left LS GR LEQ GEQ EQ NEQ
%right NOT

%%

lines: lines line { push_line_program(program, $2); }
     | %empty
     ;

line: var NL    { $$ = $1; }
    | var       { $$ = $1; }
    | expr NL   { $$ = $1; }
    | expr      { $$ = $1; }
    | cond NL   { $$ = $1; }
    | cond      { $$ = $1; }
    ;

var: ID ASSIGN expr { $$ = new_assignment($1, $3); free($1); }

expr: math_expr     { $$ = $1; }
    | bool_expr     { $$ = $1; }
    | L_B expr R_B  { $$ = $2; }
    | ID            { $$ = new_id($1); free($1); }
    ;

math_expr: expr PLUS expr       { $$ = new_node(MATH_OP_NODE, (NodeValue)Sum, $1, $3); }
         | expr MIN expr        { $$ = new_node(MATH_OP_NODE, (NodeValue)Dif, $1, $3); }
         | expr MUL expr        { $$ = new_node(MATH_OP_NODE, (NodeValue)Mul, $1, $3); }
         | expr DIV expr        { $$ = new_node(MATH_OP_NODE, (NodeValue)Div, $1, $3); }
         | NUMBER               { $$ = new_node(INT_NODE, (NodeValue)$1, NULL, NULL); }
         ;

bool_expr: expr LS expr         { $$ = new_node(BOOL_OP_NODE, (NodeValue)Ls, $1, $3); }
         | expr GR expr         { $$ = new_node(BOOL_OP_NODE, (NodeValue)Gr, $1, $3); }
         | expr LEQ expr        { $$ = new_node(BOOL_OP_NODE, (NodeValue)Leq, $1, $3); }
         | expr GEQ expr        { $$ = new_node(BOOL_OP_NODE, (NodeValue)Geq, $1, $3); }
         | expr EQ expr         { $$ = new_node(BOOL_OP_NODE, (NodeValue)Eq, $1, $3); }
         | expr NEQ expr        { $$ = new_node(BOOL_OP_NODE, (NodeValue)Neq, $1, $3); }
         | NOT expr             { $$ = new_node(BOOL_OP_NODE, (NodeValue)Not, NULL, $2); }
         | TRUE                 { $$ = new_node(BOOL_NODE, (NodeValue)$1, NULL, NULL); }
         | FALSE                { $$ = new_node(BOOL_NODE, (NodeValue)$1, NULL, NULL); }
         ;

cond: IF expr THEN expr ELSE expr    { $$ = new_conditional($2, $4, $6); }
    | IF expr DO expr ELSE expr      { $$ = new_conditional($2, $4, $6); }
    ;

%%

void yyerror(Program* program, const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

int yywrap() {
  return 1;
}

