#ifndef INTERPRETER
#define INTERPRETER

#include "hash.h"
#include "tree.h"

Node *visit_node(Bucket *scope, Node *node);

Node *visit_math_node(Bucket *scope, Node *node);

Node *visit_bool_node(Bucket *scope, Node *node);

#endif // INTERPRETER
