#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdbool.h>
#include <stddef.h>
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

// Initial capacity of a new hash table
#define HASH_TABLE_DEFAULT_CAPACITY 256

typedef struct HashMap {
    size_t length;
    size_t capacity;
    Bucket **buckets;
} HashMap;

Bucket *new_bucket();

void free_bucket(Bucket *bucket);

void free_item(Item *item);

Item *new_item(char *key, int32_t value, Item *next);

/**
 * @brief Append or updates and item in the bucket
 *
 * @param bucket
 * @param key Item key
 * @param value Item value
 * @return True if it's a new item, false if updated
 */
bool bucket_set(Bucket *bucket, char *key, int32_t value);

Item *bucket_get(Bucket *bucket, char *key);

/**
 * @brief Check if the Bucket contains a item
 *
 * @param map
 * @param key Item key
 * @return True if is contained, false otherwise
 */
bool bucket_has(Bucket *bucket, char *key);

/**
 * @brief Delete an item from the bucket
 *
 * @param bucket
 * @param key Item key
 * @return True if the item is deleted
 */
bool bucket_delete(Bucket *bucket, char *key);

/**
 * @brief Allocate a new HashMap
 *
 * @return Pointer to the HashMap
 */
HashMap *new_hash_map();

/**
 * @brief Free an HashMap
 *
 * @param map Allocated map to free
 */
void free_hash_map(HashMap *map);

/**
 * @brief Key hash function for the HashMap
 *
 * @param key
 * @return Key hash
 */
size_t hash_key(char *key);

/**
 * @brief Inserts an item into the HashMap
 *
 * @param map
 * @param key Item key
 * @param value Item vale
 */
void hm_set(HashMap *map, char *key, int32_t value);

/**
 * @brief Get an Item in the HashMap
 *
 * @param map
 * @param key Item key
 * @return Pointer to a copy of the item
 */
Item *hm_get(HashMap *map, char *key);

/**
 * @brief Check if the HashMap contains a key
 *
 * @param map
 * @param key
 * @return True if is contained, false otherwise
 */
bool hm_has(HashMap *map, char *key);

/**
 * @brief Deletes an item in the HashMap
 *
 * @param map
 * @param key Item key
 * @return True if the item is present and has been deleted
 */
bool hm_delete(HashMap *map, char *key);

/**
 * @brief Doubles the capacity of the HashMap
 *
 * @param map
 */
void hm_resize(HashMap *map);

#endif // HASH_TABLE
