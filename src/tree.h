#ifndef SYNTAX_TREE
#define SYNTAX_TREE

#include <stdbool.h>
#include <stdint.h>

typedef enum NodeType {
    INT_NODE,
    BOOL_NODE,
    MATH_OP_NODE,
    BOOL_OP_NODE,
    COND_NODE,
    ASSIGN_NODE,
    ID_NODE
} NodeType;

typedef enum MathOperator {
    Sum,
    Dif,
    Mul,
    Div,
} MathOperator;

typedef enum BoolOperator {
    Ls,
    Gr,
    Leq,
    Geq,
    Eq,
    Neq,
    Not,
} BoolOperator;

typedef union {
    char *id;
    int32_t integer;
    bool boolean;
    MathOperator math_op;
    BoolOperator bool_op;
} NodeValue;

typedef struct Node {
    NodeType type;
    NodeValue value;
    struct Node *left;
    struct Node *right;
    struct Node *condition;
} Node;

typedef enum {
    Int,
    Bool,
} Type;

Node *newNode(NodeType type, NodeValue value, Node *left, Node *right);

Node *newConditional(Node *condition, Node *left, Node *right);

Node *newAssignment(char *id, Node *expr);

Node *newID(char *id);

void printTree(Node *node);

void printNode(Node *node);

void visit(Node *node);

#endif // SYNTAX_TREE
