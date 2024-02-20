#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day4/input"
#define TEST_INPUT "day4/test_input"

typedef struct {
  int (*arr)[2];
  size_t pos;
  size_t len;
} Bucket;

typedef struct {
  Bucket **buckets;
  size_t pos;
  size_t len;
} Set;

Set *set_create(void) {
  Set *set = (Set *)malloc(sizeof(Set));
  if (set == NULL) {
    perror("malloc set");
    return NULL;
  }

  set->len = 16;
  set->pos = 0;
  set->buckets = (Bucket **)malloc(sizeof(Bucket *) * set->len);
  if (set->buckets == NULL) {
    perror("malloc set->buckets");
    return NULL;
  }

  for (int i = 0; i < (set->len); i++) {
    Bucket *b = (Bucket *)malloc(sizeof(Bucket));
    if (b == NULL) {
      perror("malloc b");
      return NULL;
    }
    b->pos = 0;
    b->len = 128;
    b->arr = (int(*)[2])malloc(sizeof(int[2]) * b->len);
    if (b->arr == NULL) {
      perror("malloc b->arr");
      return NULL;
    }
    set->buckets[i] = b;
  }
  return set;
}

void set_destroy(Set *set) {
  for (int i = 0; i < (set->len); i++) {
    Bucket *b = set->buckets[i];
    free(b->arr);
    free(b);
  }
  free(set->buckets);
  free(set);
}

size_t set_hash(Set *set, int val) { return val % set->len; }

int set_add(Set *set, int key, int val) {
  size_t idx = set_hash(set, key);
  Bucket *b = set->buckets[idx];
  if (b->pos < (b->len - 1)) {
    b->arr[b->pos][0] = key;
    b->arr[b->pos][1] = val;
    b->pos++;
    return 0;
  }
  return -1;
}

int set_has(Set *set, int key) {
  size_t idx = set_hash(set, key);
  Bucket *b = set->buckets[idx];
  if (b->pos == 0) {
    return 0;
  } else {
    for (int i = 0; i < (b->pos); i++) {
      if (b->arr[i][0] == key) {
        return 1;
      }
    }
  }
  return 0;
}

void set_print(Set *set) {
  for (int i = 0; i < (set->len); i++) {
    Bucket *b = set->buckets[i];
    for (int j = 0; j < (b->pos); j++) {
      printf("key: %d, val: %d\n", b->arr[j][0], b->arr[j][1]);
    }
  }
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  int sum = 0;
  int cards[1024] = {0};

  char line[1024];
  while (fgets(line, sizeof(line), file) != NULL) {
    Set *set = set_create();
    char *saveptr1, *saveptr2, *saveptr3;

    char *id = strtok_r(line, ":", &saveptr1);

    char id_str[strlen(id) - strlen("Card ") + 1];
    strcpy(id_str, id + strlen("Card "));

    id = strtok_r(NULL, "", &saveptr1);
    char *nums = strtok_r(id, "|", &saveptr2);
    char *win_nums = strtok_r(NULL, "", &saveptr2);

    char *num = strtok_r(nums, " ", &saveptr3);
    while (num != NULL) {
      set_add(set, atoi(num), 0);

      num = strtok_r(NULL, " ", &saveptr3);
    }

    int i = atoi(id_str);

    // paying the initialization to {1} in `int cards[1024] = {0}`
    cards[i]++;

    int multiplier = cards[i];
    sum += multiplier;

    char *win_num = strtok_r(win_nums, " ", &saveptr3);
    while (win_num != NULL) {
      if (set_has(set, atoi(win_num))) {
        cards[++i] += multiplier;
      }
      win_num = strtok_r(NULL, " ", &saveptr3);
    }
  }

  printf("answer: %d\n", sum);

  return 0;
}
