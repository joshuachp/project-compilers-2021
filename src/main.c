#include "compile.h"
#include "tree.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "parser.h"

extern FILE *yyin;

int main(void) {

    yyin = fopen("prog.txt", "r");

    // TODO: free
    Program *program = newProgram();
    int res = yyparse(program);
    if (res != 0) {
        return res;
    }
    char *prog = compileProgram(program);
    puts(prog);
    return 0;
}
