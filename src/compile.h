#ifndef COMPILE
#define COMPILE

#include "hash.h"
#include "tree.h"
#include <stddef.h>
#include <stdint.h>

typedef struct ThreeAddressCode {
    char *result;
    char *arg_1;
    char *operation;
    char *arg_2;
} ThreeAddressCode;

char *compile_program(Program *program);

char *compile_line(HashMap *scope, size_t *temp_c, Node *line);

char *compile_assignment(HashMap *scope, size_t *temp_c, Node *node);

char *compile_expression(HashMap *scope, size_t *temp_c, Node *node);

char *compile_conditional(HashMap *scope, size_t *temp_c, Node *node);

char *compile_expression_node(HashMap *scope, size_t *temp_c, char **result,
                            Node *node);

ThreeAddressCode *expression_node_to_3_ac(size_t *temp_c, Node *node,
                                          char *t_left, char *t_right);

void free_3_ac(ThreeAddressCode *code);

char *next_temp_var(size_t *temp_c);

void append_to_string(char **string, char *str);

#endif // COMPILE
