#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define INPUT "day1/input"
#define TEST_INPUT "day1/test_input"

#define LUT_LENGTH 10
char *lut[] = {"*",    "one", "two",   "three", "four",
               "five", "six", "seven", "eight", "nine"};
#define LUT_STR_MAX_LENGTH 5

int lut_digit_get(char *line, size_t pos) {
  char temp[LUT_STR_MAX_LENGTH];
  strncpy(temp, line + pos, LUT_STR_MAX_LENGTH);

  for (int i = 1; i < LUT_LENGTH; i++) {
    int lut_digit_length = strlen(lut[i]);
    if (strncmp(temp, lut[i], lut_digit_length) == 0) {
      return i;
    }
  }
  return -1;
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  int last = -1;
  int sum = 0;

  char line[1024];
  while ((fgets(line, sizeof(line), file)) != NULL) {
    size_t line_length = strlen(line);

    for (int i = 0; i < line_length; i++) {
      char byte = line[i];

      if (byte == '\n') {
        sum += last;
        last = -1;
        break;
      }

      if (isdigit(byte)) {
        int digit = byte - '0';
        if (last == -1) {
          sum += 10 * digit;
        }

        last = digit;
      } else {
        int digit = lut_digit_get(line, i);
        if (digit != -1) {
          i += strlen(lut[digit]) - 2;
          if (last == -1) {
            sum += 10 * digit;
          }

          last = digit;
        }
      }
    }
  }
  printf("sum: %d\n", sum);

  return 0;
}
