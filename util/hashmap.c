#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HSNode {
  char *key;
  char *val[2];
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
      free(curr->val[0]);
      free(curr->val[1]);
      free(curr);
      curr = next;
    }
  }
  free(hs->data);
  free(hs);
}

size_t hashmap_hashidx(HashMap *hs, char *key) {
  size_t key_sum = 0;
  for (; *key; key++) {
    key_sum += *key;
  }
  return key_sum % hs->size;
}

char **hashmap_get(HashMap *hs, char *key) {
  size_t idx = hashmap_hashidx(hs, key);

  HSNode *curr = hs->data[idx][0];
  while (curr) {
    if (strcmp(curr->key, key) == 0) {
      return curr->val;
    }
    curr = curr->next;
  }

  return NULL;
}

void hashmap_add(HashMap *hs, char *key, char *val[2]) {
  size_t idx = hashmap_hashidx(hs, key);

  HSNode *node = (HSNode *)malloc(sizeof(HSNode));
  if (!node) {
    perror("hashmaap_add");
    exit(1);
  }
  node->key = strdup(key);
  node->val[0] = strdup(val[0]);
  node->val[1] = strdup(val[1]);
  node->next = NULL;

  HSNode **bucket = hs->data[idx];
  if (!bucket[0]) {
    bucket[0] = bucket[1] = node;
  } else {
    HSNode *curr = bucket[0];
    while (curr) {
      if (strcmp(curr->key, key) == 0) {
        free(curr->val[0]);
        free(curr->val[1]);
        curr->val[0] = strdup(val[0]);
        curr->val[1] = strdup(val[1]);
        free(node->key);
        free(node->val[0]);
        free(node->val[1]);
        free(node);
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
      printf("[k:%s,v:{%s, %s}] -> ", curr->key, curr->val[0], curr->val[1]);
      printed = 1;
      curr = curr->next;
    }
    if (printed)
      printf("\n");
  }
}
