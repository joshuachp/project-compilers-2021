#ifndef SYNTAX_TREE
#define SYNTAX_TREE

typedef enum NodeType {
    Value,
    Operation,
} NodeType;

typedef enum Operator {
    SUM,
    DIF,
    MUL,
} Operator;

typedef struct Node {
    NodeType type;
    union value {
        int number;
        Operator op;
    };
    // TODO: child
} Node;

#endif // SYNTAX_TREE
