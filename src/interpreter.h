#ifndef INTERPRETER
#define INTERPRETER

#include "hash.h"
#include "tree.h"

Node *visitNode(Bucket *scope, Node *node);

Node *visitMathOpNode(Bucket *scope, Node *node);

Node *visitBoolOpNode(Bucket *scope, Node *node);

#endif // INTERPRETER
