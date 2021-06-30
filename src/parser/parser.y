%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include "tree.h"

    extern int yylex();
    extern int yyparse();

    void yyerror(const char*);
    int yywrap(void);
%}

%define parse.error verbose

%union {
    char* id;
    int32_t integer;
    bool boolean;
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
%type<integer> NUMBER
%type<boolean> TRUE FALSE

%type<node> math_expr expr cond bool_expr var

%left PLUS MIN
%right MUL DIV

%left LS GR LEQ GEQ EQ NEQ
%right NOT

%%

lines: lines line
     | %empty
     ;

line: var NL    { printTree($1); fflush(stdout); }
    | expr NL   { printTree($1); fflush(stdout); }
    | cond NL   { printTree($1); fflush(stdout); }
    | NL
    ;

var: ID ASSIGN expr { $$ = newAssignment($1, $3); }

expr: math_expr     { $$ = $1; }
    | bool_expr     { $$ = $1; }
    | L_B expr R_B  { $$ = $2; }
    | ID            { printf("ID: (%s)\n", $1); $$ = newID($1); }
    ;

math_expr: expr PLUS expr       { $$ = newNode(MATH_OP_NODE, (NodeValue)Sum, $1, $3); }
         | expr MIN expr        { $$ = newNode(MATH_OP_NODE, (NodeValue)Dif, $1, $3); }
         | expr MUL expr        { $$ = newNode(MATH_OP_NODE, (NodeValue)Mul, $1, $3); }
         | expr DIV expr        { $$ = newNode(MATH_OP_NODE, (NodeValue)Div, $1, $3); }
         | NUMBER               { $$ = newNode(INT_NODE, (NodeValue)$1, NULL, NULL); }
         ;

bool_expr: expr LS expr         { $$ = newNode(BOOL_OP_NODE, (NodeValue)Ls, $1, $3); }
         | expr GR expr         { $$ = newNode(BOOL_OP_NODE, (NodeValue)Gr, $1, $3); }
         | expr LEQ expr        { $$ = newNode(BOOL_OP_NODE, (NodeValue)Leq, $1, $3); }
         | expr GEQ expr        { $$ = newNode(BOOL_OP_NODE, (NodeValue)Geq, $1, $3); }
         | expr EQ expr         { $$ = newNode(BOOL_OP_NODE, (NodeValue)Eq, $1, $3); }
         | expr NEQ expr        { $$ = newNode(BOOL_OP_NODE, (NodeValue)Neq, $1, $3); }
         | NOT expr             { $$ = newNode(BOOL_OP_NODE, (NodeValue)Not, $2, NULL); }
         | TRUE                 { $$ = newNode(BOOL_NODE, (NodeValue)$1, NULL, NULL); }
         | FALSE                { $$ = newNode(BOOL_NODE, (NodeValue)$1, NULL, NULL); }
         ;

cond: IF expr THEN expr ELSE expr    { $$ = newConditional($2, $4, $6); }
    | IF expr DO expr ELSE expr      { $$ = newConditional($2, $4, $6); }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
    exit(1);
}

int yywrap() {
  return 1;
}

