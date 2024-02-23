#ifndef HASHMAP
#define HASHMAP

#include <stddef.h>

typedef struct HashMap HashMap;

HashMap *hashmap_create(size_t size);

void hashmap_destroy(HashMap *hs);

char **hashmap_get(HashMap *hs, char *key);

void hashmap_add(HashMap *hs, char *key, char *val[2]);

void hashmap_print(HashMap *hs);

#endif // !HASHMAP
