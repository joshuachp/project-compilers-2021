#include "hash.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

Bucket *new_bucket() {
    Bucket *bucket = malloc(sizeof(Bucket));
    bucket->head = NULL;
    bucket->tail = NULL;
    return bucket;
}

void free_bucket(Bucket *bucket) {
    if (bucket == NULL)
        return;
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
    if (item == NULL)
        return;
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

bool bucket_set(Bucket *bucket, char *key, int32_t value) {
    // Check if the bucket contains the item
    Item *item = bucket_get(bucket, key);
    if (item == NULL) {
        // If it doesn't contain, allocate a new one
        item = new_item(key, value, NULL);
        // Check if bucket is empty and set new item as head and tail
        if (bucket->head == NULL && bucket->tail == NULL) {
            bucket->head = item;
            bucket->tail = item;
        } else {
            // Append the item to the bucket
            bucket->tail->next = item;
            bucket->tail = item;
        }
        // Return new item appended
        return true;
    } else {
        // Update the item value
        item->value = value;
        return false;
    }
}

Item *bucket_get(Bucket *bucket, char *key) {
    if (bucket == NULL) {
        return NULL;
    }
    // Cycle throw the items
    Item *item = bucket->head;
    while (item != NULL) {
        if (strcmp(key, item->key) == 0) {
            // Return the copy of the items
            Item *copy = new_item(item->key, item->value, NULL);
            return copy;
        }
        item = item->next;
    }
    return NULL;
}

bool bucket_has(Bucket *bucket, char *key) {
    if (bucket == NULL) {
        return false;
    }
    // Cycle throw the items
    Item *item = bucket->head;
    while (item != NULL) {
        if (strcmp(key, item->key) == 0) {
            // Return the copy of the items
            return true;
        }
        item = item->next;
    }
    return false;
}

bool bucket_delete(Bucket *bucket, char *key) {
    if (bucket == NULL) {
        return false;
    }
    // Cycle throw the items, keeping track of the previous
    Item *item = bucket->head;
    Item *prev = NULL;
    while (item != NULL) {
        if (strcmp(key, item->key) == 0) {
            // Check if the item is head or tail and replace with the
            // appropriate pointer
            if (item == bucket->head) {
                bucket->head = item->next;
            }
            if (item == bucket->tail) {
                bucket->tail = prev;
            }
            // Check if we have a prev item to set the correct next
            if (prev != NULL) {
                prev->next = item->next;
            }
            // Free the item
            free_item(item);
            item = NULL;
            return true;
        }
        prev = item;
        item = item->next;
    }
    return false;
}

HashMap *new_hash_map() {
    HashMap *map = malloc(sizeof(HashMap));
    map->length = 0;
    map->capacity = HASH_TABLE_DEFAULT_CAPACITY;
    map->buckets = calloc(HASH_TABLE_DEFAULT_CAPACITY, sizeof(Bucket *));
    return map;
}

void free_hash_map(HashMap *map) {
    if (map == NULL)
        return;
    // Cycle to all the allocated buckets to check if we need to free them
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL) {
            free_bucket(map->buckets[i]);
            map->buckets[i] = NULL;
        }
    }
    // Free the buckets array and then the map
    free(map->buckets);
    map->buckets = NULL;
    free(map);
}

size_t hash_key(char *key) {
    // Polynomial with x = 31
    size_t hash = 0;
    for (size_t i = 0; i < strlen(key); i++) {
        hash += 31 * hash + key[i];
    }
    return hash;
}

void hm_resize(HashMap *map) {
    Bucket **old_buckets = map->buckets;
    size_t old_capacity = map->capacity;
    // Allocate with double the capacity
    map->buckets = calloc(2 * old_capacity, sizeof(Bucket *));
    map->capacity = 2 * old_capacity;
    map->length = 0;
    // Re insert all the elements in the new buckets
    for (size_t i = 0; i < old_capacity; i++) {
        if (old_buckets[i] != NULL) {
            // Reinsert the item
            Item *item = old_buckets[i]->head;
            while (item != NULL) {
                hm_set(map, item->key, item->value);
                item = item->next;
            }
            // Free the reinserted bucket
            free_bucket(old_buckets[i]);
            old_buckets[i] = NULL;
        }
    }
    // Free the old buckets array
    free(old_buckets);
    old_buckets = NULL;
}

void hm_set(HashMap *map, char *key, int32_t value) {
    // Check the capacity, capacity == length we are sure there will be
    // collisions
    if (map->length + 1 >= map->capacity) {
        hm_resize(map);
    }
    // Get the hash of the key modulo the HashMap capacity
    size_t index = hash_key(key) % map->capacity;
    if (map->buckets[index] == NULL) {
        map->buckets[index] = new_bucket();
    }
    if (bucket_set(map->buckets[index], key, value)) {
        map->length += 1;
    }
}

Item *hm_get(HashMap *map, char *key) {
    // Get the hash of the key modulo the HashMap capacity
    size_t index = hash_key(key) % map->capacity;
    if (map->buckets[index] == NULL) {
        return NULL;
    }
    // Get the item copy or NULL
    return bucket_get(map->buckets[index], key);
}

bool hm_has(HashMap *map, char *key) {
    // Get the hash of the key modulo the HashMap capacity
    size_t index = hash_key(key) % map->capacity;
    if (map->buckets[index] == NULL) {
        return false;
    }
    return bucket_has(map->buckets[index], key);
}

bool hm_delete(HashMap *map, char *key) {
    // Get the hash of the key modulo the HashMap capacity
    size_t index = hash_key(key) % map->capacity;
    if (map->buckets[index] == NULL) {
        return false;
    }
    if (bucket_delete(map->buckets[index], key)) {
        map->length -= 1;
        return true;
    }
    return false;
}
