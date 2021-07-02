#include "compile.h"
#include "hash.h"
#include "tree.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "parser.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yyparse(Program *program);
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern FILE *yyin;

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
                    printTree(program->lines[i]);
                    puts("");
                }

                // Free
                freeProgram(program);
                program = NULL;
            }
        } while (line_read != NULL);
        return 0;
    }

    return 0;
}
