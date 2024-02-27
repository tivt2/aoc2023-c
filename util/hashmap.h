#ifndef HASHMAP
#define HASHMAP

#include <stddef.h>

typedef struct {
  char *ipt;
  int *backup;
  size_t len;
} Key;

typedef struct {
  size_t out;
} Val;

typedef struct HSNode {
  Key *key;
  Val *val;
} HSNode;

typedef struct HashMap HashMap;

HashMap *hashmap_create(size_t size);

void hashmap_destroy(HashMap *hs);

HSNode *hashmap_get(HashMap *hs, Key *key);

void hashmap_add(HashMap *hs, Key *key, Val *val);

void hashmap_print(HashMap *hs);

#endif // !HASHMAP
