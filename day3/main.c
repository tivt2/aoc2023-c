#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_INPUT "day3/test_input"
#define INPUT "day3/input"

#define NOT_SYMBOL '.'

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

#define is_symbol(char) (char != NOT_SYMBOL && !isdigit(char))

int is_adjacent_to_symbol(int start, int end, char rows[3][1024]) {
  char *middle_row = rows[1];
  start = max(start - 1, 0);
  end = min(end + 1, strlen(middle_row) - 2); // -2 to remove '\n'

  if (start > 0) {
    char middle_left = middle_row[start];
    if (is_symbol(middle_left)) {
      return 1;
    }
  }
  if (end < strlen(middle_row) - 1) {
    char middle_right = middle_row[end];
    if (is_symbol(middle_right)) {
      return 1;
    }
  }

  char *above_row = strlen(rows[0]) > 0 ? rows[0] : NULL;
  char *under_row = strlen(rows[2]) > 0 ? rows[2] : NULL;

  for (int i = start; i <= end; i++) {
    if (above_row != NULL) {
      char above_byte = above_row[i];
      if (is_symbol(above_byte)) {
        return 1;
      }
    }
    if (under_row != NULL) {
      char under_byte = under_row[i];
      if (is_symbol(under_byte)) {
        return 1;
      }
    }
  }
  return 0;
}

void center_row_process(char rows[3][1024], int *sum) {
  char *row = rows[1];
  for (int i = 0; i < strlen(row); i++) {
    char byte = row[i];
    if (isdigit(byte)) {
      int num_start_idx = i;
      while (isdigit(byte)) {
        byte = row[++i];
      }

      char num_str[i - num_start_idx];
      strncpy(num_str, row + num_start_idx, i - num_start_idx);
      num_str[i - num_start_idx] = '\0';
      if (is_adjacent_to_symbol(num_start_idx, i - 1, rows)) {
        *sum += atoi(num_str);
      }
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

  char cur_rows[3][1024] = {"\0"};
  while (fgets(cur_rows[2], sizeof(cur_rows[2]), file) != NULL) {
    if (strlen(cur_rows[1]) != 0) {
      center_row_process(cur_rows, &sum);
    }

    strcpy(cur_rows[0], cur_rows[1]);
    strcpy(cur_rows[1], cur_rows[2]);
  }
  strcpy(cur_rows[2], "\0");

  center_row_process(cur_rows, &sum);

  printf("answer: %d\n", sum);

  return 0;
}
