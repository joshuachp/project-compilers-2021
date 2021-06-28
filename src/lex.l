%{
    #include <stdlib.h>
    #include "main.tab.h"

    extern void yyerror(const char*);
%}

NUMBER      [0-9]+
OPERATOR    [-+*]
R_BRACK     "("
L_BRACK     "("

%%

{NUMBER} {
    yylval = atoi(yytext);
    return NUMBER;
}

{OPERATOR}  { return *yytext; }

{R_BRACK}   { return *yytext; }

{L_BRACK}   { return *yytext; }

[\n\r]      { return *yytext; }

[ \t]       /* skip whitespace */

%%
