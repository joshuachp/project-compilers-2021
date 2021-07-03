#ifndef INTERPRETER
#define INTERPRETER

#include "hash.h"
#include "tree.h"

/**
 * @brief Visit a node tree
 *
 * @param scope Value HashMap
 * @param node Node tree
 * @return Result of the nodes or NULL
 */
Node *visit_node(HashMap *scope, Node *node);

/**
 * @brief Visit a node math operation tree
 *
 * @param scope Value HashMap
 * @param node Math operation node tree
 * @return Integer node as result or NULL
 */
Node *visit_math_node(HashMap *scope, Node *node);

/**
 * @brief Visit a node boolean operation tree
 *
 * @param scope Value HashMap
 * @param node Bool operation node tree
 * @return Bool node as result or NULL
 */
Node *visit_bool_node(HashMap *scope, Node *node);

#endif // INTERPRETER
