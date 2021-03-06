#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

Node *new_node(NodeType type, NodeValue value, Node *left, Node *right) {
    Node *node = malloc(sizeof(Node));
    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;
    node->condition = NULL;
    return node;
}

Node *new_conditional(Node *condition, Node *left, Node *right) {
    Node *node = malloc(sizeof(Node));
    node->type = COND_NODE;
    node->left = left;
    node->right = right;
    node->condition = condition;
    return node;
}

Node *new_assignment(char *id, Node *expr) {
    Node *node = malloc(sizeof(Node));
    node->type = ASSIGN_NODE;
    node->value.id = strdup(id);
    node->right = expr;
    node->left = NULL;
    node->condition = NULL;
    return node;
}

Node *new_id(char *id) {
    Node *node = malloc(sizeof(Node));
    node->type = ID_NODE;
    node->value.id = strdup(id);
    node->left = NULL;
    node->right = NULL;
    node->condition = NULL;
    return node;
}

void free_tree(Node *node) {
    if (node == NULL) {
        return;
    }
    free_tree(node->left);
    free_tree(node->right);
    free_tree(node->condition);
    if (node->type == ID_NODE) {
        free(node->value.id);
    }
    free(node);
}

void print_node(Node *node) {
    if (node == NULL) {
        fprintf(stderr, "Error printing node: NULL node\n");
        exit(1);
    } else if (node->type == INT_NODE) {
        printf(" %d", node->value.value);
    } else if (node->type == BOOL_NODE) {
        if (node->value.value) {
            printf(" True");
        } else {
            printf(" False");
        }
    } else if (node->type == MATH_OP_NODE) {
        switch (node->value.math_op) {
            case Sum:
                printf(" +");
                break;
            case Dif:
                printf(" -");
                break;
            case Mul:
                printf(" *");
                break;
            case Div:
                printf(" /");
                break;
        }
    } else if (node->type == BOOL_OP_NODE) {
        switch (node->value.bool_op) {
            case Ls:
                printf(" <");
                break;
            case Gr:
                printf(" >");
                break;
            case Leq:
                printf(" <=");
                break;
            case Geq:
                printf(" >=");
                break;
            case Eq:
                printf(" ==");
                break;
            case Neq:
                printf(" !=");
                break;
            case Not:
                printf(" !");
                break;
        }
    } else if (node->type == COND_NODE) {
        printf(" if");
        print_tree(node->condition);
        printf(" else");
    } else if (node->type == ASSIGN_NODE) {
        printf("%s =", node->value.id);
    } else if (node->type == ID_NODE) {
        printf(" %s", node->value.id);
    }
}

void print_tree(Node *node) {
    if (node == NULL)
        return;
    print_tree(node->left);
    print_node(node);
    print_tree(node->right);
}

Program *new_program() {
    Program *program = malloc(sizeof(Program));
    program->lines = calloc(PROGRAM_DEFAULT_CAPACITY, sizeof(Node));
    program->capacity = PROGRAM_DEFAULT_CAPACITY;
    program->length = 0;
    return program;
}

void free_program(Program *program) {
    for (size_t i = 0; i < program->length; i++) {
        free_tree(program->lines[i]);
    }
    free(program->lines);
    free(program);
}

void push_line_program(Program *program, Node *line) {
    // Check if the array has enough capacity
    if (program->length + 1 > program->capacity) {
        // Reallocate with double the capacity
        program->lines =
            realloc(program->lines, 2 * program->capacity * sizeof(Node));
    }
    // Append the line and increase the length of the array
    program->lines[program->length] = line;
    program->length += 1;
}

char *node_operation_to_string(Node *node) {
    char *op;
    if (node->type == MATH_OP_NODE) {
        switch (node->value.math_op) {
            case Sum:
                op = strdup("+");
                break;
            case Dif:
                op = strdup("-");
                break;
            case Mul:
                op = strdup("*");
                break;
            case Div:
                op = strdup("/");
                break;
        }
    } else if (node->type == BOOL_OP_NODE) {
        switch (node->value.bool_op) {
            case Ls:
                op = strdup("<");
                break;
            case Gr:
                op = strdup(">");
                break;
            case Leq:
                op = strdup("<=");
                break;
            case Geq:
                op = strdup(">=");
                break;
            case Eq:
                op = strdup("==");
                break;
            case Neq:
                op = strdup("!=");
                break;
            case Not:
                op = strdup("!");
                break;
        }
    } else {
        op = NULL;
    }
    return op;
}
