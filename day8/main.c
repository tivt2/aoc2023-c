#include "../util/hashmap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day8/input"
#define TEST_INPUT "day8/test_input"

#define max(x, y) (x > y ? x : y)
#define min(x, y) (x < y ? x : y)

#define BUFFER_SIZE 1024

uint64_t path_start(char *inst, char *start, HashMap *hs) {
  uint64_t steps = 0;

  char *str = inst;
  uint64_t i = 0;

  char *pos = start;
  while (1) {
    char instruction = *(str + i);

    if (instruction != 'L' && instruction != 'R') {
      steps += i;
      str = inst;
      i = 0;
      instruction = *(str + i);
    }

    char **vals = hashmap_get(hs, pos);
    if (instruction == 'L') {
      pos = vals[0];
    } else if (instruction == 'R') {
      pos = vals[1];
    }

    if (pos[2] == 'Z') {
      steps += i + 1;
      break;
    }

    i++;
  }

  return steps;
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  char *inst = (char *)malloc(BUFFER_SIZE);
  if (!fgets(inst, BUFFER_SIZE, file)) {
    perror("instructions");
    return 1;
  }

  char *line = (char *)malloc(BUFFER_SIZE);
  if (!fgets(line, BUFFER_SIZE, file)) {
    perror("empty line");
    return 1;
  }

  HashMap *hs = hashmap_create(64);
  char **starts = (char **)malloc(BUFFER_SIZE * BUFFER_SIZE);
  size_t starts_len = 0;

  while (fgets(line, BUFFER_SIZE, file)) {
    char id[4];
    char left[4];
    char right[4];

    if (sscanf(line, "%3s = (%3s, %3s)", id, left, right) != 3) {
      perror("sscanf");
      return 1;
    }

    if (id[2] == 'A') {
      starts[starts_len++] = strdup(id);
    }

    char *val[2];
    val[0] = malloc(strlen(left) + 1);
    val[1] = malloc(strlen(right) + 1);
    if (val[0] == NULL || val[1] == NULL) {
      perror("left right");
      return 1;
    }

    strcpy(val[0], left);
    strcpy(val[1], right);

    hashmap_add(hs, id, val);
  }

  char **pos = starts;
  uint64_t steps_lcm = path_start(inst, pos[0], hs);

  for (uint64_t i = 1; i < starts_len; i++) {
    uint64_t steps = path_start(inst, pos[i], hs);

    uint64_t gcd = 1;
    for (uint64_t j = gcd; j <= steps && j <= steps_lcm; j++) {
      if (steps % j == 0 && steps_lcm % j == 0) {
        gcd = j;
      }
    }

    steps_lcm = (steps_lcm * steps) / gcd;
  }

  printf("answer: %lu\n", steps_lcm);

  hashmap_destroy(hs);
  free(line);
  fclose(file);

  return 0;
}
