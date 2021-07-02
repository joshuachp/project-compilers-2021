#include "compile.h"
#include "hash.h"
#include "interpreter.h"
#include "tree.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <readline/history.h>
#include <readline/readline.h>
#include <string.h>

#include "parser.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yyparse(Program *program);
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern FILE *yyin;

void printHelp() {
    puts("calc [OPTIONS] [ARG]\n"
         " Compiler and interpreter for an infix calculator\n"
         " with variables, boolean operations and conditionals\n"
         "\n"
         "ARG\n"
         "\tFILE_NAME name of the file to compile or interpret\n"
         "\t          if it's missing it will open a REPL\n"
         "OPTIONS\n"
         "\t-i        interprete the input instead of compiling\n"
         "\t--help    prints this help message\n");
}

int interpreter_shell() {
    char *line_read = NULL;
    YY_BUFFER_STATE buffer = NULL;
    Program *program = NULL;
    Bucket *scope = newBucket();
    do {
        if (line_read) {
            free(line_read);
            line_read = (char *)NULL;
        }
        line_read = readline("$ ");
        if (line_read != NULL && *line_read) {
            add_history(line_read);

            // Parse a string
            program = newProgram();
            buffer = yy_scan_string(line_read);
            yyparse(program);
            yy_delete_buffer(buffer);
            buffer = NULL;

            for (size_t i = 0; i < program->length; i++) {
                Node *node = visitNode(scope, program->lines[i]);
                if (node != NULL) {
                    printNode(node);
                    puts("");
                    freeTree(node);
                }
            }
            // Free
            freeProgram(program);
            program = NULL;
        }
    } while (line_read != NULL);
    return 0;
}

int interpret_file() {
    Program *program = newProgram();
    Bucket *scope = newBucket();

    int res = yyparse(program);
    if (res != 0) {
        return res;
    }
    fclose(yyin);

    for (size_t i = 0; i < program->length; i++) {
        Node *node = visitNode(scope, program->lines[i]);
        if (node != NULL) {
            printNode(node);
            puts("");
            freeTree(node);
        }
    }

    freeProgram(program);
    freeBucket(scope);
    return 0;
}

int compile_file() {
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
}

int main(int argc, char **argv) {
    if (argc > 3) {
        printHelp();
        return 1;
    }

    if (argc == 1) {
        return interpreter_shell();
    }

    bool interpret = false;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // Parse options
            if (strcmp(argv[i], "--help") == 0) {
                printHelp();
                return 0;
            } else if (strcmp(argv[i], "-i") == 0) {
                interpret = true;
            } else {
                fprintf(stderr, "Unrecognized option %s, see --help\n", argv[i]);
                return 1;
            }
        } else {
            if (yyin != NULL) {
                fprintf(stderr, "To many file argument passed, see the --help\n");
                return 1;
            }
            yyin = fopen(argv[i], "r");
            if (yyin == NULL) {
                perror("Error reading input file");
                return 1;
            }
        }
    }

    if (yyin == NULL) {
        return interpreter_shell();
    } else if (interpret) {
        return interpret_file();
    } else {
        return compile_file();
    }

    return 0;
}
