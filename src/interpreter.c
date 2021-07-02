#include "interpreter.h"
#include "hash.h"
#include "tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *visitNode(Bucket *scope, Node *node) {
    if (node == NULL) {
        return NULL;
    }
    Node *res = NULL;
    switch (node->type) {
        case INT_NODE:
        case BOOL_NODE:
            res = malloc(sizeof(Node));
            memcpy(res, node, sizeof(Node));
            return res;
        case MATH_OP_NODE:
            res = visitMathOpNode(scope, node);
            return res;
        case BOOL_OP_NODE:
            res = visitBoolOpNode(scope, node);
            return res;
        case COND_NODE: {
            Node *condition = visitBoolOpNode(scope, node->condition);
            // Return if error happened downstream
            if (condition == NULL) {
                return NULL;
            }
            if (condition->value.boolean) {
                res = visitNode(scope, node->left);
                printNode(res);
            } else {
                res = visitNode(scope, node->right);
                printNode(res);
            }
            freeTree(condition);
            condition = NULL;
            freeTree(res);
            res = NULL;
            return NULL;
        }
        case ASSIGN_NODE: {
            res = visitNode(scope, node->right);
            setItem(node->value.id, res->value.integer, scope);
            freeTree(res);
            res = NULL;
            return NULL;
        }
        case ID_NODE: {
            Item *item = getItem(node->value.id, scope);
            if (item == NULL) {
                fprintf(stderr, "Error value not defined: %s\n", node->value.id);
                return NULL;
            }
            res = newNode(INT_NODE, (NodeValue)item->value, NULL, NULL);
            return res;
        }
    }
}

Node *visitMathOpNode(Bucket *scope, Node *node) {
    Node *left = visitNode(scope, node->left);
    Node *right = visitNode(scope, node->right);
    Node *res = newNode(INT_NODE, (NodeValue)0, NULL, NULL);

    // Return NULL if erro happened downstream
    if (left == NULL || right == NULL) {
        freeTree(left);
        freeTree(right);
        left = NULL;
        right = NULL;
        return NULL;
    }

    switch (node->value.math_op) {
        case Sum:
            res->value.integer = left->value.integer + right->value.integer;
            break;
        case Dif:
            res->value.integer = left->value.integer - right->value.integer;
            break;
        case Mul:
            res->value.integer = left->value.integer * right->value.integer;
            break;
        case Div:
            res->value.integer = left->value.integer / right->value.integer;
            break;
    }
    // Free
    freeTree(left);
    freeTree(right);
    left = NULL;
    right = NULL;
    return res;
}

Node *visitBoolOpNode(Bucket *scope, Node *node) {
    Node *res = newNode(BOOL_NODE, (NodeValue) false, NULL, NULL);
    Node *left = visitNode(scope, node->left);
    Node *right = visitNode(scope, node->right);

    // Return NULL if erro happened downstream
    if (right == NULL || (node->type != Not && left == NULL)) {
        freeTree(left);
        freeTree(right);
        left = NULL;
        right = NULL;
        return NULL;
    }

    switch (node->value.bool_op) {
        case Ls:
            res->value.boolean = left->value.boolean < right->value.boolean;
            break;
        case Gr:
            res->value.boolean = left->value.boolean > right->value.boolean;
            break;
        case Leq:
            res->value.boolean = left->value.boolean <= right->value.boolean;
            break;
        case Geq:
            res->value.boolean = left->value.boolean >= right->value.boolean;
            break;
        case Eq:
            res->value.boolean = left->value.boolean == right->value.boolean;
            break;
        case Neq:
            res->value.boolean = left->value.boolean != right->value.boolean;
            break;
        case Not:
            res->value.boolean = !right->value.boolean;
            break;
    }
    // Free
    freeTree(left);
    freeTree(right);
    left = NULL;
    right = NULL;
    return res;
}
