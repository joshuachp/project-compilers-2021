#include "compile.h"
#include "hash.h"
#include "tree.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *compile_program(Program *program) {
    HashMap *scope = new_hash_map();
    Node *line;
    size_t max_temp = 0;

    char *main = NULL;

    for (size_t i = 0; i < program->length; i++) {
        size_t temp = 0;
        line = program->lines[i];
        char *program_line = compile_line(scope, &temp, line);
        append_to_string(&main, program_line);
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
        append_to_string(&final, temp);
        free(temp);
    }
    append_to_string(&final, main);
    append_to_string(&final, "}\n");

    // Free
    free_hash_map(scope);
    free(main);

    return final;
}

char *compile_line(HashMap *scope, size_t *temp_c, Node *line) {
    switch (line->type) {
        // Assign
        case ASSIGN_NODE:
            return compile_assignment(scope, temp_c, line);
        // Expr
        case BOOL_NODE:
        case MATH_OP_NODE:
        case BOOL_OP_NODE:
        case ID_NODE:
            return compile_expression(scope, temp_c, line);
        // Cond
        case COND_NODE:
            return compile_conditional(scope, temp_c, line);
            break;
        default:
            print_tree(line);
            fprintf(stderr, "Something went wrong in compiling above line\n");
            exit(1);
    }
}

char *compile_assignment(HashMap *scope, size_t *temp_c, Node *node) {
    char *result = NULL;
    if (!hm_has(scope, node->value.id)) {
        // Declare variable
        size_t buff_size = snprintf(NULL, 0, "int %s;\n", node->value.id) + 1;
        char *declaration = malloc(buff_size);
        snprintf(declaration, buff_size, "int %s;\n", node->value.id);
        append_to_string(&result, declaration);
        // Free
        free(declaration);
    }
    // Assign variable
    char *temp_res =
        compile_expression_node(scope, temp_c, &result, node->right);
    size_t buff_size =
        snprintf(NULL, 0, "%s = %s;\n", node->value.id, temp_res) + 1;
    char *assignment = malloc(buff_size);
    snprintf(assignment, buff_size, "%s = %s;\n", node->value.id, temp_res);
    append_to_string(&result, assignment);
    // Set variable in scope (real value doesn't matter)
    hm_set(scope, node->value.id, 0);
    // Free temp_res
    free(temp_res);
    free(assignment);
    return result;
}

char *compile_expression(HashMap *scope, size_t *temp_c, Node *node) {
    char *result = NULL;
    char *temp_res = compile_expression_node(scope, temp_c, &result, node);
    // Print result
    size_t buff_size =
        snprintf(NULL, 0, "printf(\"= %%d\\n\", %s);\n", temp_res) + 1;
    char *print = malloc(buff_size);
    snprintf(print, buff_size, "printf(\"= %%d\\n\", %s);\n", temp_res);
    append_to_string(&result, print);
    // Free
    free(temp_res);
    free(print);
    return result;
}

char *compile_conditional(HashMap *scope, size_t *temp_c, Node *node) {
    char *result = NULL;
    size_t max_temp = 0;
    size_t temp = 0;
    // If
    char *temp_con =
        compile_expression_node(scope, &max_temp, &result, node->condition);
    char *body_expr = compile_expression(scope, &temp, node->left);
    if (temp > max_temp) {
        max_temp = temp;
    }
    temp = 0;
    char *other_expr = compile_expression(scope, &temp, node->right);
    if (temp > max_temp) {
        max_temp = temp;
    }
    // Create if string
    size_t buff_size = snprintf(NULL, 0, "if (%s) {\n%s} else {\n%s}\n",
                                temp_con, body_expr, other_expr) +
                       1;
    char *condition = malloc(buff_size);
    snprintf(condition, buff_size, "if (%s) {\n%s} else {\n%s}\n", temp_con,
             body_expr, other_expr);
    append_to_string(&result, condition);
    // Free
    free(temp_con);
    free(condition);
    free(body_expr);
    free(other_expr);
    // Set max temp
    *temp_c = max_temp;
    return result;
}

char *compile_expression_node(HashMap *scope, size_t *temp_c, char **result,
                              Node *node) {
    if (node == NULL) {
        return NULL;
    }

    char *temp_l = compile_expression_node(scope, temp_c, result, node->left);
    char *temp_r = compile_expression_node(scope, temp_c, result, node->right);

    // Check if variable initialized
    if (node->type == ID_NODE) {
        if (!hm_has(scope, node->value.id)) {
            fprintf(stderr, "Error variable not declared: %s\n",
                    node->value.id);
            exit(1);
        }
        return strdup(node->value.id);
    }

    ThreeAddressCode *code =
        expression_node_to_3_ac(temp_c, node, temp_l, temp_r);
    char *temp_res = strdup(code->result);

    size_t buff_size = snprintf(NULL, 0, "%s = %s %s %s;\n", code->result,
                                code->arg_1 == NULL ? "" : code->arg_1,
                                code->operation == NULL ? "" : code->operation,
                                code->arg_2 == NULL ? "" : code->arg_2);
    buff_size += 1;
    char *string = malloc(buff_size);
    snprintf(string, buff_size, "%s = %s %s %s;\n", code->result,
             code->arg_1 == NULL ? "" : code->arg_1,
             code->operation == NULL ? "" : code->operation,
             code->arg_2 == NULL ? "" : code->arg_2);
    append_to_string(result, string);

    // Free code
    free_3_ac(code);
    free(temp_l);
    free(temp_r);
    free(string);
    return temp_res;
}

ThreeAddressCode *expression_node_to_3_ac(size_t *temp_c, Node *node,
                                          char *temp_l, char *temp_r) {
    ThreeAddressCode *code = malloc(sizeof(ThreeAddressCode));
    size_t buff_size;
    switch (node->type) {
        case INT_NODE: {
            code->operation = NULL;
            buff_size = snprintf(NULL, 0, "%d", node->value.value) + 1;
            code->arg_1 = malloc(buff_size);
            snprintf(code->arg_1, buff_size, "%d", node->value.value);
            code->arg_2 = NULL;
            code->result = next_temp_var(temp_c);
            break;
        }
        case BOOL_NODE:
            code->operation = NULL;
            code->arg_1 = strdup(node->value.value ? "1" : "0");
            code->arg_2 = NULL;
            code->result = next_temp_var(temp_c);
            break;
        case MATH_OP_NODE:
        case BOOL_OP_NODE:
            code->operation = node_operation_to_string(node);
            code->arg_1 = strdup(temp_l);
            code->arg_2 = strdup(temp_r);
            code->result = next_temp_var(temp_c);
            break;
        case ID_NODE:
            // This code is not reached by visiting the node but I still added
            // it here
            code->operation = NULL;
            code->arg_1 = NULL;
            code->arg_2 = NULL;
            code->result = strdup(node->value.id);
            break;
        default:
            // Conditional and assignment not an expression
            print_tree(node);
            fprintf(stderr, "Error node not an expression\n");
            exit(1);
    }
    return code;
}

void free_3_ac(ThreeAddressCode *code) {
    if (code == NULL)
        return;
    free(code->result);
    free(code->arg_1);
    free(code->operation);
    free(code->arg_2);
    free(code);
}

char *next_temp_var(size_t *temp_c) {
    size_t buff_size = snprintf(NULL, 0, "temp_%zu", *temp_c) + 1;
    char *temp = malloc(buff_size);
    snprintf(temp, buff_size, "temp_%zu", *temp_c);
    (*temp_c) += 1;
    return temp;
}

void append_to_string(char **string, char *str) {
    if (*string == NULL) {
        *string = strdup(str);
    } else {
        size_t size = strlen(*string) + strlen(str) + 1;
        *string = realloc(*string, size);
        strcat(*string, str);
    }
}
