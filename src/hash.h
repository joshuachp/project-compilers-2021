#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Item {
    char *key;
    int32_t value;
    struct Item *next;
} Item;

typedef struct Bucket {
    Item *head;
    Item *tail;
} Bucket;

Bucket *newBucket();

void freeBucket(Bucket *bucket);

void freeItem(Item *item);

Item *newItem(char *key, int32_t value, Item *next);

Item *setItem(char *key, int32_t value, Bucket *bucket);

Item *getItem(char *key, Bucket *bucket);

#endif // HASH_TABLE
