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

char *compile_line(size_t *temp_c, Node *line, Bucket *scope);

char *compile_assignment(size_t *temp_c, Node *node, Bucket *scope);

char *compile_expression(size_t *temp_c, Node *node, Bucket *scope);

char *compile_conditional(size_t *temp_c, Node *node, Bucket *scope);

char *visit_expression_node(size_t *temp_c, char **result, Bucket *scope, Node *node);

ThreeAddressCode *expression_node_to_3_ac(size_t *temp_c, Node *node, char *t_left, char *t_right);

void free_3_ac(ThreeAddressCode *code);

char *next_temp_var(size_t *temp_c);

void append_to_string(char **string, char *str);

#endif // COMPILE
