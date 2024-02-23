#include "../util/hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day8/input"
#define TEST_INPUT "day8/test_input"

#define BUFFER_SIZE 1024

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

  HashMap *hs = hashmap_create(32);

  while (fgets(line, BUFFER_SIZE, file)) {
    char id[4];
    char left[4];
    char right[4];

    if (sscanf(line, "%3s = (%3s, %3s)", id, left, right) != 3) {
      perror("sscanf");
      return 1;
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

  size_t steps = 0;

  char *str = inst;
  size_t i = 0;

  char *curr = "AAA";
  while (1) {
    char instruction = *(str + i);

    if (instruction != 'L' && instruction != 'R') {
      steps += i;
      i = 0;
      str = inst;
      continue;
    }

    char **vals = hashmap_get(hs, curr);
    if (instruction == 'L') {
      curr = vals[0];
    } else if (instruction == 'R') {
      curr = vals[1];
    }
    if (strcmp(curr, "ZZZ") == 0) {
      steps += i + 1;
      break;
    }

    i++;
  }

  printf("answer: %zu\n", steps);

  hashmap_destroy(hs);
  free(line);
  fclose(file);

  return 0;
}
