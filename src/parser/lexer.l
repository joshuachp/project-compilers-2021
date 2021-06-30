%{
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "parser.h"

    extern void yyerror(const char*);
%}

NUMBER [0-9]+

PLUS "+"
MIN  "-"
MUL  "*"
DIV  "/"

LS  "<"
GR  ">"
LEQ "<="
GEQ ">="
EQ  "=="
NEQ "!="
NOT "!"

TRUE  "True"
FALSE "False"

L_B "("
R_B ")"

NL [\n\r]

IF if
THEN then
DO do
ELSE else

ID [A-Za-z_]+

%%

{NUMBER} { yylval.integer = atoi(yytext); return NUMBER; }

{PLUS} { return PLUS; }
{MIN}  { return MIN; }
{MUL}  { return MUL; }
{DIV}  { return DIV; }

{LS}  { return LS; }
{GR}  { return GR; }
{LEQ} { return LEQ; }
{GEQ} { return GEQ; }
{EQ}  { return EQ; }
{NEQ} { return NEQ; }
{NOT} { return NOT; }

{TRUE}  { yylval.boolean = true; return TRUE; }
{FALSE} { yylval.boolean = false; return FALSE; }

{L_B} { return L_B; }
{R_B} { return R_B; }

{NL} { return NL; }

{IF}    { return IF; }
{THEN}  { return THEN; }
{DO}    { return DO; }
{ELSE}  { return ELSE; }

{ID} { yylval.id = yytext; return ID; }

[ \t]               /* skip white-space */

%%
