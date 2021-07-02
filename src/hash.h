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

typedef struct HashMap {
    size_t length;
    size_t capacity;
    Bucket **buckets;
} HashMap;

Bucket *new_bucket();

void free_bucket(Bucket *bucket);

void free_item(Item *item);

Item *new_item(char *key, int32_t value, Item *next);

Item *set_item(char *key, int32_t value, Bucket *bucket);

Item *get_item(char *key, Bucket *bucket);

#endif // HASH_TABLE
