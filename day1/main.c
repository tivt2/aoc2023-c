#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define INPUT "day1/input"
#define TEST_INPUT "day1/test_input"

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
      }
    }
  }
  printf("sum: %d\n", sum);

  return 0;
}
