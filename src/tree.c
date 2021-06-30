#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

Node *newNode(NodeType type, NodeValue value, Node *left, Node *right) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;
    node->condition = NULL;
    return node;
}

Node *newConditional(Node *condition, Node *left, Node *right) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = COND;
    node->left = left;
    node->right = right;
    node->condition = condition;
    return node;
}

void newAssignment(char *id, Node *expr) {
    // TODO
}

void printNode(Node *node) {
    if (node == NULL) {
        fprintf(stderr, "Error printing node: NULL node\n");
        exit(1);
    } else if (node->type == INT) {
        printf(" %d", node->value.integer);
    } else if (node->type == BOOL) {
        if (node->value.boolean) {
            printf(" True");
        } else {
            printf(" False");
        }
    } else if (node->type == MATH_OP) {
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
    } else if (node->type == BOOL_OP) {
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
    } else if (node->type == COND) {
        printf(" if");
        printTree(node->condition);
        printf(" else");
    }
}

void printTree(Node *node) {
    if (node == NULL)
        return;
    printTree(node->left);
    printNode(node);
    printTree(node->right);
}
