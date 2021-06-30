#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef union {
    bool b;
    int32_t i;
} Value;

typedef enum {
    Bool,
    Int,
} Type;

typedef struct Item {
    char *key;
    Value value;
    Type type;
    struct Item *next;
} Item;

typedef struct Bucket {
    Item *head;
    Item *tail;
} Bucket;

Item *newItem(char *key, Value value, Item *next);

Item *pushItem(char *key, Value value, Bucket *bucket);

Item *searchItem(char *key, Bucket *bucket);

#endif // HASH_TABLE
