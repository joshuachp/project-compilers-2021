#ifndef SYNTAX_TREE
#define SYNTAX_TREE

#include <stdbool.h>

typedef enum NodeType { INT, BOOL, MATH_OP, BOOL_OP, COND, ASSIGN } NodeType;

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
    int integer;
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

typedef union {
    int integer;
    bool boolean;
} Value;

typedef struct Var {
    Type type;
    Value value;
} Var;

Node *newNode(NodeType type, NodeValue value, Node *left, Node *right);

Node *newConditional(Node *condition, Node *left, Node *right);

void newAssignment(char* id, Node* expr);

void printTree(Node *node);

void printNode(Node *node);

void visit(Node *node);

#endif // SYNTAX_TREE
