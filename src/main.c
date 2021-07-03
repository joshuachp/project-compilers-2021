#include "compile.h"
#include "hash.h"
#include "interpreter.h"
#include "parser.h"
#include "tree.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <readline/history.h>
#include <readline/readline.h>
#include <string.h>

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yyparse(Program *program);
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern FILE *yyin;

/**
 * @brief Print help message for command line
 *
 */
void print_help() {
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

/**
 * @brief Open the interpreted interactive shell
 *
 * @return Status code
 */
int interpreter_shell() {
    // Line read as input
    char *line_read = NULL;
    // The parsed program
    Program *program = NULL;
    // The global scope
    HashMap *scope = new_hash_map();
    // Token buffer
    YY_BUFFER_STATE buffer = NULL;
    do {
        // Free the line allocated in the previous cycle
        if (line_read) {
            free(line_read);
            line_read = (char *)NULL;
        }
        // Read a line
        line_read = readline("$ ");
        // If the line is not null parse it
        if (line_read != NULL && *line_read) {
            // Add the line to the history
            add_history(line_read);

            // Parse a string
            program = new_program();
            buffer = yy_scan_string(line_read);
            int ret = yyparse(program);
            yy_delete_buffer(buffer);
            buffer = NULL;
            // Check the return value of the parser
            if (ret == 0) {
                // Visit each line and execute them
                for (size_t i = 0; i < program->length; i++) {
                    Node *node = visit_node(scope, program->lines[i]);
                    // Print if the node returned a result
                    if (node != NULL) {
                        print_node(node);
                        puts("");
                        free_tree(node);
                    }
                }
            }
            // Free program
            free_program(program);
            program = NULL;
        }
    } while (line_read != NULL);
    free_hash_map(scope);
    scope = NULL;
    return 0;
}

/**
 * @brief Execute the selected file
 *
 * @return Status code
 */
int interpret_file() {
    Program *program = new_program();
    // Global scope
    HashMap *scope = new_hash_map();
    // Parse the file and check for parse error
    int res = yyparse(program);
    if (res != 0) {
        return res;
    }
    fclose(yyin);
    // Interpret each line of the file
    for (size_t i = 0; i < program->length; i++) {
        Node *node = visit_node(scope, program->lines[i]);
        if (node != NULL) {
            print_node(node);
            puts("");
            free_tree(node);
            node = NULL;
        }
    }
    free_program(program);
    free_hash_map(scope);
    program = NULL;
    scope = NULL;
    return 0;
}

/**
 * @brief Compile the selected file to c code
 *
 * @return Status code
 */
int compile_file() {
    Program *program = new_program();
    // Parse the file and check for parse error
    int res = yyparse(program);
    if (res != 0) {
        return res;
    }
    fclose(yyin);

    // Compile the program and print the result
    char *prog = compile_program(program);
    puts(prog);

    free_program(program);
    free(prog);
    program = NULL;
    prog = NULL;

    return 0;
}

/**
 * @brief Parse the command line arguments
 *
 * @param argc Argument counter
 * @param argv Argument array
 * @return True if interpret parameter is passed
 */
bool parse_arguments(int argc, char **argv) {
    bool interpret = false;
    // Cycle throw the arguments except the program name
    for (int i = 1; i < argc; i++) {
        // If the argument starts with a - it's an options
        if (argv[i][0] == '-') {
            // Parse the option
            if (strcmp(argv[i], "--help") == 0) {
                print_help();
                return 0;
            } else if (strcmp(argv[i], "-i") == 0) {
                // Set the interpreter flag
                interpret = true;
            } else {
                fprintf(stderr, "Unrecognized option %s, see --help\n",
                        argv[i]);
                return 1;
            }
        } else {
            // If the input file is already set throw an error
            if (yyin != NULL) {
                fprintf(stderr,
                        "To many file argument passed, see the --help\n");
                return 1;
            }
            // Open the file and check for errors
            yyin = fopen(argv[i], "r");
            if (yyin == NULL) {
                perror("Error reading input file");
                return 1;
            }
        }
    }
    return interpret;
}

int main(int argc, char **argv) {
    // Print help if to many arguments
    if (argc > 3) {
        print_help();
        return 1;
    }
    // Open the shell if only the program name is passed
    if (argc == 1) {
        return interpreter_shell();
    }
    // Parse the arguments
    bool interpret = parse_arguments(argc, argv);
    // If no file is passed open the shell otherwise interpret/compile it
    if (yyin == NULL) {
        return interpreter_shell();
    } else if (interpret) {
        return interpret_file();
    } else {
        return compile_file();
    }

    return 0;
}
