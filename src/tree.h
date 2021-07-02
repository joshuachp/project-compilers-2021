#ifndef SYNTAX_TREE
#define SYNTAX_TREE

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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
    int32_t value;
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

// Initial capacity of the Program lines array
#define PROGRAM_DEFAULT_CAPACITY 10

typedef struct Program {
    size_t capacity;
    size_t length;
    Node **lines;
} Program;

/**
 * @brief Allocate a new node and returns the pointer
 *
 * @param type Type of the node
 * @param value Node value
 * @param left Left node for operations, can be NULL
 * @param right Right node for operation, can be NULL
 * @return Pointer to the node
 */
Node *new_node(NodeType type, NodeValue value, Node *left, Node *right);

/**
 * @brief Allocates a new conditional node and returns a pointer to it. It will
 * require the condition node
 *
 * @param condition The condition for the if
 * @param left If body expression
 * @param right Else body expression
 * @return Pointer to the conditional expression
 */
Node *new_conditional(Node *condition, Node *left, Node *right);

/**
 * @brief Allocate a new assignment node, it will require only and id and value
 *
 * @param id Id of the variable
 * @param expr Expression of the value assignment
 * @return Pointer to the assignment node
 */
Node *new_assignment(char *id, Node *expr);

/**
 * @brief Allocate and ID node, it will only need the id key
 *
 * @param id Key of the id
 * @return Pointer to the node
 */
Node *new_id(char *id);

/**
 * @brief Free a node and all of its children
 *
 * @param node The node to free
 */
void free_tree(Node *node);

/**
 * @brief Prints a node and it's children
 *
 * @param node Node tree to print
 */
void print_tree(Node *node);

/**
 * @brief Prints a single node
 *
 * @param node Node to print
 */
void print_node(Node *node);

/**
 * @brief Allocate a new Program and return the pointer to it
 *
 * @return Pinter to the allocated program
 */
Program *new_program();

/**
 * @brief Frees a program
 *
 * @param program Program to free
 */
void free_program(Program *program);

/**
 * @brief Append a line to the program, resizing the array if needed
 *
 * @param program
 * @param line
 */
void push_line_program(Program *program, Node *line);

/**
 * @brief Returns a string of the operation (value) for a MATH_OP_NODE or
 * BOOL_OP_NODE
 *
 * @param node
 * @return String of the operation
 */
char *node_operation_to_string(Node *node);

#endif // SYNTAX_TREE
