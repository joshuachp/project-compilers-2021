#include "interpreter.h"
#include "hash.h"
#include "tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *visit_node(Bucket *scope, Node *node) {
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
            res = visit_math_node(scope, node);
            return res;
        case BOOL_OP_NODE:
            res = visit_bool_node(scope, node);
            return res;
        case COND_NODE: {
            Node *condition = visit_node(scope, node->condition);
            // Return if error happened downstream
            if (condition == NULL) {
                return NULL;
            }
            if (condition->value.value) {
                res = visit_node(scope, node->left);
            } else {
                res = visit_node(scope, node->right);
            }
            free_tree(condition);
            condition = NULL;
            return res;
        }
        case ASSIGN_NODE: {
            res = visit_node(scope, node->right);
            set_item(node->value.id, res->value.value, scope);
            free_tree(res);
            res = NULL;
            return NULL;
        }
        case ID_NODE: {
            Item *item = get_item(node->value.id, scope);
            if (item == NULL) {
                fprintf(stderr, "Error value not defined: %s\n",
                        node->value.id);
                return NULL;
            }
            res = new_node(INT_NODE, (NodeValue)item->value, NULL, NULL);
            return res;
        }
    }
}

Node *visit_math_node(Bucket *scope, Node *node) {
    Node *left = visit_node(scope, node->left);
    Node *right = visit_node(scope, node->right);
    Node *res = new_node(INT_NODE, (NodeValue)0, NULL, NULL);

    // Return NULL if erro happened downstream
    if (left == NULL || right == NULL) {
        free_tree(left);
        free_tree(right);
        left = NULL;
        right = NULL;
        return NULL;
    }

    switch (node->value.math_op) {
        case Sum:
            res->value.value = left->value.value + right->value.value;
            break;
        case Dif:
            res->value.value = left->value.value - right->value.value;
            break;
        case Mul:
            res->value.value = left->value.value * right->value.value;
            break;
        case Div:
            res->value.value = left->value.value / right->value.value;
            break;
    }
    // Free
    free_tree(left);
    free_tree(right);
    left = NULL;
    right = NULL;
    return res;
}

Node *visit_bool_node(Bucket *scope, Node *node) {
    Node *res = new_node(BOOL_NODE, (NodeValue) false, NULL, NULL);
    Node *left = visit_node(scope, node->left);
    Node *right = visit_node(scope, node->right);

    // Return NULL if erro happened downstream
    if (right == NULL || (node->value.bool_op != Not && left == NULL)) {
        free_tree(left);
        free_tree(right);
        left = NULL;
        right = NULL;
        return NULL;
    }

    switch (node->value.bool_op) {
        case Ls:
            res->value.value = left->value.value < right->value.value;
            break;
        case Gr:
            res->value.value = left->value.value > right->value.value;
            break;
        case Leq:
            res->value.value = left->value.value <= right->value.value;
            break;
        case Geq:
            res->value.value = left->value.value >= right->value.value;
            break;
        case Eq:
            res->value.value = left->value.value == right->value.value;
            break;
        case Neq:
            res->value.value = left->value.value != right->value.value;
            break;
        case Not:
            res->value.value = !right->value.value;
            break;
    }
    // Free
    free_tree(left);
    free_tree(right);
    left = NULL;
    right = NULL;
    return res;
}
