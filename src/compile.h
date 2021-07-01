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

char *compileProgram(Program *program);

char *compileLine(size_t *temp_c, Node *line, Bucket *scope);

char *compileAssignment(size_t *temp_c, Node *node, Bucket *scope);

char *compileExpression(size_t *temp_c, Node *node, Bucket *scope);

char *compileConditional(size_t *temp_c, Node *node, Bucket *scope);

char *visitExpressionNode(size_t *temp_c, char **result, Bucket *scope, Node *node);

ThreeAddressCode *expressionNodeTo3AC(size_t *temp_c, Node *node, char *t_left, char *t_right);

void free3AC(ThreeAddressCode *code);

char *nextTempVar(size_t *temp_c);

void appendToString(char **string, char *str);

#endif // COMPILE
