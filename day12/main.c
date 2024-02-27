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

size_t spring_resolve(char *input, int *backup, size_t len) {
  if (*input == '\0') {
    if (len == 0) {
      return 1;
    }
    return 0;
  } else if (len == 0) {
    return !contains(input, "#", strlen(input));
  }

  size_t out = 0;

  if (*input == '.' || *input == '?') {
    out += spring_resolve(input + 1, backup, len);
  }

  if (*input == '#' || *input == '?') {
    int bkp = backup[0];
    if (bkp <= strlen(input) && !contains(input, ".", bkp) &&
        (bkp == strlen(input) || input[bkp] != '#')) {
      out += spring_resolve(input + bkp + 1, backup + 1, len - 1);
    }
  }

  return out;
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  size_t sum = 0;

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

    sum += spring_resolve(input, backup, len);
  }

  printf("answer: %zu\n", sum);

  return 0;
}
