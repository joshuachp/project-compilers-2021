#include "hash.h"
#include <stdlib.h>
#include <string.h>

Bucket *new_bucket() {
    Bucket *bucket = malloc(sizeof(Bucket));
    bucket->head = NULL;
    bucket->tail = NULL;
    return bucket;
}

void free_bucket(Bucket *bucket) {
    Item *item = bucket->head;
    Item *next;
    while (item != NULL) {
        next = item->next;
        free_item(item);
        item = next;
        next = NULL;
    }
    free(bucket);
}

void free_item(Item *item) {
    free(item->key);
    free(item);
}

Item *new_item(char *key, int32_t value, Item *next) {
    Item *item = malloc(sizeof(Item));
    // Own the string
    item->key = strdup(key);
    item->value = value;
    item->next = next;
    return item;
}

Item *set_item(char *key, int32_t value, Bucket *bucket) {
    Item *item = get_item(key, bucket);
    if (item == NULL) {
        item = new_item(key, value, NULL);
        if (bucket->head == NULL && bucket->tail == NULL) {
            bucket->head = item;
            bucket->tail = item;
        } else {
            bucket->tail->next = item;
            bucket->tail = item;
        }
    } else {
        item->value = value;
    }
    return item;
}

Item *get_item(char *key, Bucket *bucket) {
    Item *item = bucket->head;
    while (item != NULL) {
        if (strcmp(key, item->key) == 0) {
            return item;
        }
        item = item->next;
    }
    return NULL;
}
