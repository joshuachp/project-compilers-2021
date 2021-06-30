#include "hash.h"
#include <stdlib.h>
#include <string.h>

Item *newItem(char *key, Value value, Item *next) {
    Item *item = malloc(sizeof(Item));
    // Own the string
    item->key = malloc(strlen(key) + 1);
    strcpy(item->key, key);
    item->value = value;
    item->next = next;
    return item;
}

Item *pushItem(char *key, Value value, Bucket *bucket) {
    Item *item = newItem(key, value, NULL);
    bucket->tail->next = item;
    bucket->tail = item;
    return item;
}

Item *searchValue(char *key, Bucket *bucket) {
    Item *item = bucket->tail;
    while (item != NULL) {
        if (strcmp(key, item->key) == 0) {
            return item;
        }
        item = item->next;
    }
    return NULL;
}
