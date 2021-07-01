#include "compile.h"
#include "hash.h"
#include "tree.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *compileProgram(Program *program) {
    Bucket *scope = newBucket();
    Node *line;
    size_t max_temp = 0;

    char *main = NULL;

    for (size_t i = 0; i < program->length; i++) {
        size_t temp = 0;
        line = program->lines[i];
        char *program_line = compileLine(&temp, line, scope);
        appendToString(&main, program_line);
        free(program_line);
        // Check number of temp variables
        if (temp > max_temp) {
            max_temp = temp;
        }
    }

    char *final = strdup("#include <stdio.h>\n\nvoid main() {\n");

    for (size_t i = 0; i < max_temp; i++) {
        size_t buff_size = snprintf(NULL, 0, "int temp_%lu;\n", i) + 1;
        char *temp = malloc(buff_size);
        snprintf(temp, buff_size, "int temp_%lu;\n", i);
        appendToString(&final, temp);
        free(temp);
    }
    appendToString(&final, main);
    appendToString(&final, "}\n");

    // Free
    freeBucket(scope);
    free(main);

    return final;
}

char *compileLine(size_t *temp_c, Node *line, Bucket *scope) {
    switch (line->type) {
        // Assign
        case ASSIGN_NODE:
            return compileAssignment(temp_c, line, scope);
        // Expr
        case BOOL_NODE:
        case MATH_OP_NODE:
        case BOOL_OP_NODE:
        case ID_NODE:
            return compileExpression(temp_c, line, scope);
        // Cond
        case COND_NODE:
            return compileConditional(temp_c, line, scope);
            break;
        default:
            printTree(line);
            fprintf(stderr, "Something went wrong in compiling above line\n");
            exit(1);
    }
}

char *compileAssignment(size_t *temp_c, Node *node, Bucket *scope) {
    Item *var = getItem(node->value.id, scope);
    char *result = NULL;
    if (var == NULL) {
        // Declare variable
        size_t buff_size = snprintf(NULL, 0, "int %s;\n", node->value.id) + 1;
        char *declaration = malloc(buff_size);
        snprintf(declaration, buff_size, "int %s;\n", node->value.id);
        appendToString(&result, declaration);
        // Free
        free(declaration);
    }
    // Assign variable
    char *temp_res = visitExpressionNode(temp_c, &result, scope, node->right);
    size_t buff_size = snprintf(NULL, 0, "%s = %s;\n", node->value.id, temp_res) + 1;
    char *assignment = malloc(buff_size);
    snprintf(assignment, buff_size, "%s = %s;\n", node->value.id, temp_res);
    appendToString(&result, assignment);
    // Set variable in scope (real value doesn't matter)
    setItem(node->value.id, 0, scope);
    // Free temp_res
    free(temp_res);
    free(assignment);
    return result;
}

char *compileExpression(size_t *temp_c, Node *node, Bucket *scope) {
    char *result = NULL;
    char *temp_res = visitExpressionNode(temp_c, &result, scope, node);
    // Print result
    size_t buff_size = snprintf(NULL, 0, "printf(\"= %%d\\n\", %s);\n", temp_res) + 1;
    char *print = malloc(buff_size);
    snprintf(print, buff_size, "printf(\"= %%d\\n\", %s);\n", temp_res);
    appendToString(&result, print);
    // Free
    free(temp_res);
    free(print);
    return result;
}

char *compileConditional(size_t *temp_c, Node *node, Bucket *scope) {
    char *result = NULL;
    size_t max_temp = 0;
    size_t temp = 0;
    // If
    char *temp_con = visitExpressionNode(&max_temp, &result, scope, node->condition);
    char *body_expr = compileExpression(&temp, node->left, scope);
    if (temp > max_temp) {
        max_temp = temp;
    }
    temp = 0;
    char *other_expr = compileExpression(&temp, node->right, scope);
    if (temp > max_temp) {
        max_temp = temp;
    }
    // Create if string
    size_t buff_size =
        snprintf(NULL, 0, "if (%s) {\n%s} else {\n%s}\n", temp_con, body_expr, other_expr) + 1;
    char *condition = malloc(buff_size);
    snprintf(condition, buff_size, "if (%s) {\n%s} else {\n%s}\n", temp_con, body_expr, other_expr);
    appendToString(&result, condition);
    // Free
    free(temp_con);
    free(condition);
    free(body_expr);
    free(other_expr);
    // Set max temp
    *temp_c = max_temp;
    return result;
}

char *visitExpressionNode(size_t *temp_c, char **result, Bucket *scope, Node *node) {
    if (node == NULL) {
        return NULL;
    }

    char *temp_l = visitExpressionNode(temp_c, result, scope, node->left);
    char *temp_r = visitExpressionNode(temp_c, result, scope, node->right);

    // Check if variable initialized
    if (node->type == ID_NODE) {
        if (getItem(node->value.id, scope) == NULL) {
            fprintf(stderr, "Error variable not declared: %s\n", node->value.id);
            exit(1);
        }
        return node->value.id;
    }

    ThreeAddressCode *code = expressionNodeTo3AC(temp_c, node, temp_l, temp_r);
    char *temp_res = strdup(code->result);

    size_t buff_size = snprintf(
        NULL, 0, "%s = %s %s %s;\n", code->result, code->arg_1 == NULL ? "" : code->arg_1,
        code->operation == NULL ? "" : code->operation, code->arg_2 == NULL ? "" : code->arg_2);
    buff_size += 1;
    char *string = malloc(buff_size);
    snprintf(string, buff_size, "%s = %s %s %s;\n", code->result,
             code->arg_1 == NULL ? "" : code->arg_1, code->operation == NULL ? "" : code->operation,
             code->arg_2 == NULL ? "" : code->arg_2);
    appendToString(result, string);

    // Free code
    free3AC(code);
    free(temp_l);
    free(temp_r);
    free(string);
    return temp_res;
}

ThreeAddressCode *expressionNodeTo3AC(size_t *temp_c, Node *node, char *temp_l, char *temp_r) {
    ThreeAddressCode *code = malloc(sizeof(ThreeAddressCode));
    size_t buff_size;
    switch (node->type) {
        case INT_NODE: {
            code->operation = NULL;
            buff_size = snprintf(NULL, 0, "%d", node->value.integer) + 1;
            code->arg_1 = malloc(buff_size);
            snprintf(code->arg_1, buff_size, "%d", node->value.integer);
            code->arg_2 = NULL;
            code->result = nextTempVar(temp_c);
            break;
        }
        case BOOL_NODE:
            code->operation = NULL;
            code->arg_1 = strdup(node->value.boolean ? "true" : "false");
            code->arg_2 = NULL;
            code->result = nextTempVar(temp_c);
            break;
        case MATH_OP_NODE:
        case BOOL_OP_NODE:
            code->operation = nodeOperationToString(node);
            code->arg_1 = strdup(temp_l);
            code->arg_2 = strdup(temp_r);
            code->result = nextTempVar(temp_c);
            break;
        case ID_NODE:
            // This code is not reached by visiting the node but I still added it here
            code->operation = NULL;
            code->arg_1 = NULL;
            code->arg_2 = NULL;
            code->result = strdup(node->value.id);
            break;
        default:
            // Conditional and assignment not an expression
            printTree(node);
            fprintf(stderr, "Error node not an expression\n");
            exit(1);
    }
    return code;
}

void free3AC(ThreeAddressCode *code) {
    free(code->result);
    free(code->arg_1);
    free(code->operation);
    free(code->arg_2);
    free(code);
}

char *nextTempVar(size_t *temp_c) {
    size_t buff_size = snprintf(NULL, 0, "temp_%zu", *temp_c) + 1;
    char *temp = malloc(buff_size);
    snprintf(temp, buff_size, "temp_%zu", *temp_c);
    (*temp_c) += 1;
    return temp;
}

void appendToString(char **string, char *str) {
    if (*string == NULL) {
        *string = strdup(str);
    } else {
        size_t size = strlen(*string) + strlen(str) + 2;
        *string = realloc(*string, size);
        strcat(*string, str);
    }
}
