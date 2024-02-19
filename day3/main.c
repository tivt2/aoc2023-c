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

int number_extract_idxs(int idx, char *str) {
  int idxes[2] = {-1, -1};
  char byte = str[idx];
  if (!isdigit(byte)) {
    return 0;
  }

  idxes[0] = idx;
  idxes[1] = idx;
  int finished_dir[2] = {0, 0};
  while (!(finished_dir[0] && finished_dir[1])) {
    if (!finished_dir[0]) {
      char left_byte = str[idxes[0] - 1];
      if (idxes[0] == 0) {
        finished_dir[0] = 1;
      } else if (isdigit(left_byte)) {
        idxes[0]--;
      } else {
        finished_dir[0] = 1;
      }
    }

    if (!finished_dir[1]) {
      char right_byte = str[idxes[1] + 1];
      if (idxes[1] == strlen(str)) {
        finished_dir[1] = 1;
      } else if (isdigit(right_byte)) {
        idxes[1]++;
      } else {
        finished_dir[1] = 1;
      }
    }
  }

  int len = idxes[1] - idxes[0] + 1;
  char num_str[len + 1];
  strncpy(num_str, str + idxes[0], len);
  num_str[len] = '\0';
  return atoi(num_str);
}

void center_row_process(char rows[3][1024], int *sum) {
  char *row = rows[1];
  for (int i = 0; i < strlen(row); i++) {
    char byte = row[i];
    if (byte == '*') {
      int gears[2] = {-1, -1};

      int num = 0;
      // middle left
      if ((num = number_extract_idxs(i - 1, rows[1])) != 0) {
        if (gears[0] == -1) {
          gears[0] = num;
        } else if (gears[1] == -1) {
          gears[1] = num;
        }
      }

      // middle right
      if ((num = number_extract_idxs(i + 1, rows[1])) != 0) {
        if (gears[0] == -1) {
          gears[0] = num;
        } else if (gears[1] == -1) {
          gears[1] = num;
        }
      }

      // top middle
      if ((num = number_extract_idxs(i, rows[0])) != 0) {
        if (gears[0] == -1) {
          gears[0] = num;
        } else if (gears[1] == -1) {
          gears[1] = num;
        } else {
          continue;
        }
      } else {
        // top left
        if ((num = number_extract_idxs(i - 1, rows[0])) != 0) {
          if (gears[0] == -1) {
            gears[0] = num;
          } else if (gears[1] == -1) {
            gears[1] = num;
          } else {
            continue;
          }
        }
        // top right
        if ((num = number_extract_idxs(i + 1, rows[0])) != 0) {
          if (gears[0] == -1) {
            gears[0] = num;
          } else if (gears[1] == -1) {
            gears[1] = num;
          } else {
            continue;
          }
        }
      }

      // bottom middle
      if ((num = number_extract_idxs(i, rows[2])) != 0) {
        if (gears[0] == -1) {
          gears[0] = num;
        } else if (gears[1] == -1) {
          gears[1] = num;
        } else {
          continue;
        }
      } else {
        // bottom left
        if ((num = number_extract_idxs(i - 1, rows[2])) != 0) {
          if (gears[0] == -1) {
            gears[0] = num;
          } else if (gears[1] == -1) {
            gears[1] = num;
          } else {
            continue;
          }
        }
        // bottom right
        if ((num = number_extract_idxs(i + 1, rows[2])) != 0) {
          if (gears[0] == -1) {
            gears[0] = num;
          } else if (gears[1] == -1) {
            gears[1] = num;
          } else {
            continue;
          }
        }
      }

      if (gears[0] != -1 && gears[1] != -1) {
        *sum += gears[0] * gears[1];
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
