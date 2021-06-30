#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

Node *newNode(NodeType type, NodeValue value, Node *left, Node *right) {
    Node *node = malloc(sizeof(Node));
    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;
    node->condition = NULL;
    return node;
}

Node *newConditional(Node *condition, Node *left, Node *right) {
    Node *node = malloc(sizeof(Node));
    node->type = COND_NODE;
    node->left = left;
    node->right = right;
    node->condition = condition;
    return node;
}

Node *newAssignment(char *id, Node *expr) {
    Node *node = malloc(sizeof(Node));
    node->type = ASSIGN_NODE;
    node->value.id = malloc(strlen(id) + 1);
    strcpy(node->value.id, id);
    node->right = expr;
    node->left = NULL;
    node->condition = NULL;
   return node;
}

Node *newID(char *id) {
    Node *node = malloc(sizeof(Node));
    node->type = ID_NODE;
    node->value.id = malloc(strlen(id) + 1);
    strcpy(node->value.id, id);
    node->left = NULL;
    node->right = NULL;
    node->condition = NULL;
    return node;
}

void printNode(Node *node) {
    if (node == NULL) {
        fprintf(stderr, "Error printing node: NULL node\n");
        exit(1);
    } else if (node->type == INT_NODE) {
        printf(" %d", node->value.integer);
    } else if (node->type == BOOL_NODE) {
        if (node->value.boolean) {
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
        printTree(node->condition);
        printf(" else");
    } else if (node->type == ASSIGN_NODE) {
        printf("%s =", node->value.id);
    } else if (node->type == ID_NODE) {
        printf(" %s", node->value.id);
    }
}

void printTree(Node *node) {
    if (node == NULL)
        return;
    printTree(node->left);
    printNode(node);
    printTree(node->right);
}
