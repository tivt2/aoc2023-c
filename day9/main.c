#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day9/input"
#define TEST_INPUT "day9/test_input"

#define BUFFER_SIZE 1024

int64_t number_produce_rec(int64_t *nums, size_t len) {
  if (len == 1) {
    return nums[0];
  }

  int64_t incs[len - 1];
  for (int i = len - 1; i > 0; i--) {
    incs[i - 1] = nums[i] - nums[i - 1];
  }

  int64_t next_inc = number_produce_rec(incs, len - 1);
  return nums[len - 1] + next_inc;
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  int64_t sum = 0;

  char *line = malloc(BUFFER_SIZE);
  while (fgets(line, BUFFER_SIZE - 1, file)) {

    int64_t *nums = (int64_t *)malloc(sizeof(int64_t) * 64);
    size_t nums_len = 0;

    char *num_str = strtok(line, " ");
    while (num_str) {
      nums[nums_len++] = atoll(num_str);
      num_str = strtok(NULL, " ");
    }

    sum += number_produce_rec(nums, nums_len);
  }

  printf("answer: %ld\n", sum);

  return 0;
}
