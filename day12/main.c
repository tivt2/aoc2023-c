#include "../util/hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day12/input"
#define TEST_INPUT "day12/test_input"

#define BUFFER_SIZE 1024

int contains(char *haystack, char *needle, size_t n) {
  for (size_t i = 0; i < n && haystack[i] != '\0'; i++) {
    for (size_t j = 0; needle[j] != '\0'; j++) {
      if (haystack[i] == needle[j]) {
        return 1;
      }
    }
  }
  return 0;
}

size_t spring_resolve(char *input, int *backup, size_t len, HashMap *cache) {
  if (*input == '\0') {
    if (len == 0) {
      return 1;
    }
    return 0;
  } else if (len == 0) {
    return !contains(input, "#", strlen(input));
  }

  Key *key = (Key *)malloc(sizeof(Key));
  key->ipt = input;
  key->backup = backup;
  key->len = len;
  HSNode *node = hashmap_get(cache, key);
  if (node) {
    free(key);
    return node->val->out;
  }

  size_t out = 0;

  if (*input == '.' || *input == '?') {
    out += spring_resolve(input + 1, backup, len, cache);
  }

  if (*input == '#' || *input == '?') {
    int bkp = backup[0];
    if (bkp <= strlen(input) && !contains(input, ".", bkp) &&
        (bkp == strlen(input) || input[bkp] != '#')) {
      out += spring_resolve(input + bkp + 1, backup + 1, len - 1, cache);
    }
  }

  Val *val = (Val *)malloc(sizeof(Val));
  val->out = out;
  hashmap_add(cache, key, val);

  return out;
}

int main(void) {
  FILE *file = fopen(TEST_INPUT, "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  size_t sum = 0;
  int i = 0;

  HashMap *cache = hashmap_create(BUFFER_SIZE * 10);

  char *line = malloc(BUFFER_SIZE);
  while (fgets(line, BUFFER_SIZE - 1, file)) {
    char *input = strtok(line, " ,");

    int backup[BUFFER_SIZE];
    size_t len = 0;

    char *n_str = strtok(NULL, " ,");
    while (n_str) {
      backup[len++] = atoi(n_str);
      n_str = strtok(NULL, " ,");
    }

    const char separator = '?';
    char new_str[strlen(input) * 5 + 4];
    strcpy(new_str, input);
    for (int i = 0; i < 5; i++) {
      if (i != 0) {
        strcat(new_str, &separator);
        strcat(new_str, input);
      }
      memcpy(&backup[i * len], backup, sizeof(size_t) * len);
    }
    len = len * 5;

    printf("ipt: %s, ", new_str);
    printf("bkp: ");
    for (int i = 0; i < len; i++) {
      printf("%d, ", backup[i]);
    }
    printf("len: %zu\n", len);

    sum += spring_resolve(new_str, backup, len, cache);
  }

  printf("answer: %zu\n", sum);

  hashmap_destroy(cache);
  fclose(file);

  return 0;
}
