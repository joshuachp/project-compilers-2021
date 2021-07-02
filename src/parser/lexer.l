%{
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "parser.h"
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

IF if
THEN then
DO do
ELSE else

ID [A-Za-z][A-Za-z0-9]*
ASSIGN "="

NL [\n\r]

%%

{NUMBER} { yylval.value = atoi(yytext); return NUMBER; }

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

{TRUE}  { yylval.value = true; return TRUE; }
{FALSE} { yylval.value = false; return FALSE; }

{L_B} { return L_B; }
{R_B} { return R_B; }

{IF}    { return IF; }
{THEN}  { return THEN; }
{DO}    { return DO; }
{ELSE}  { return ELSE; }

{ID} { yylval.id = strdup(yytext); return ID; }
{ASSIGN} { return ASSIGN; }

{NL} { return NL; }

[ \t] /* skip white-space */

%%
