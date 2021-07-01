#include "compile.h"
#include "tree.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "parser.h"

extern FILE *yyin;

void compiler(Program *program) {}

void interpreter(Program *program) {}

int main(int argc, char **argv) {

    if (argc > 2) {
        fprintf(stderr, "To many parameters parameters\n");
        return 1;
    }

    if (argc == 2) {
        yyin = fopen(argv[1], "r");

        if (yyin == NULL) {
            perror("Error reading input file");
            return 1;
        }

        Program *program = newProgram();
        int res = yyparse(program);
        if (res != 0) {
            return res;
        }
        fclose(yyin);

        char *prog = compileProgram(program);
        puts(prog);

        freeProgram(program);
        free(prog);

        return 0;
    } else {
        // TODO interpreter
    }

    return 0;
}
