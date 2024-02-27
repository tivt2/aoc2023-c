#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  struct HSNode *next;
} HSNode;

typedef struct {
  HSNode *(*data)[2];
  size_t size;
} HashMap;

HashMap *hashmap_create(size_t size) {
  HashMap *hs = (HashMap *)malloc(sizeof(HashMap));
  if (!hs) {
    return NULL;
  }
  hs->size = size;
  hs->data = (HSNode * (*)[2]) malloc(sizeof(HSNode *[2]) * size);

  return hs;
}

void hashmap_destroy(HashMap *hs) {
  for (size_t i = 0; i < (hs->size); i++) {
    HSNode *curr = hs->data[i][0];
    while (curr) {
      HSNode *next = curr->next;
      free(curr->key);
      free(curr->val);
      free(curr);
      curr = next;
    }
  }
  free(hs->data);
  free(hs);
}

size_t hashmap_hashidx(HashMap *hs, Key *key) {
  size_t key_sum = 0;
  for (int i = 0; key->ipt[i]; i++) {
    key_sum += key->ipt[i] + key->backup[i % key->len] * i;
  }
  return key_sum % hs->size;
}

HSNode *hashmap_get(HashMap *hs, Key *key) {
  size_t idx = hashmap_hashidx(hs, key);

  HSNode *curr = hs->data[idx][0];
  while (curr) {
    if (strcmp(curr->key->ipt, key->ipt) == 0) {
      return curr;
    }
    curr = curr->next;
  }

  return NULL;
}

void hashmap_add(HashMap *hs, Key *key, Val *val) {
  size_t idx = hashmap_hashidx(hs, key);

  HSNode *node = (HSNode *)malloc(sizeof(HSNode));
  if (!node) {
    perror("hashmaap_add");
    exit(1);
  }
  node->key = key;
  node->val = val;
  node->next = NULL;

  HSNode **bucket = hs->data[idx];
  if (!bucket[0]) {
    bucket[0] = bucket[1] = node;
  } else {
    HSNode *curr = bucket[0];
    while (curr) {
      if (strcmp(curr->key->ipt, key->ipt) == 0) {
        curr->val = val;
        return;
      }
      curr = curr->next;
    }

    bucket[1]->next = node;
    bucket[1] = node;
  }
}

void hashmap_print(HashMap *hs) {
  for (size_t i = 0; i < (hs->size); i++) {
    HSNode *curr = hs->data[i][0];
    int printed = 0;
    while (curr) {
      printf("[k: %s,v: %zu] -> ", curr->key->ipt, curr->val->out);
      printed = 1;
      curr = curr->next;
    }
    if (printed)
      printf("\n");
  }
}
