#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day11/input"
#define TEST_INPUT "day11/test_input"

#define BUFFER_SIZE 512

size_t galaxies_solve(size_t (*glxs)[2], size_t len_glx) {
  size_t sum = 0;

  for (int i = 0; i < len_glx; i++) {
    size_t start_x = glxs[i][0];
    size_t start_y = glxs[i][1];

    for (int j = i + 1; j < len_glx; j++) {
      size_t target_x = glxs[j][0];
      size_t target_y = glxs[j][1];

      while (start_x != target_x) {
        sum++;
        if (target_x > start_x) {
          target_x--;
        } else {
          target_x++;
        }
      }
      while (start_y != target_y) {
        sum++;
        if (target_y > start_y) {
          target_y--;
        } else {
          target_y++;
        }
      }
    }
  }

  return sum;
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  char **map = (char **)malloc(sizeof(char *) * BUFFER_SIZE);
  if (map == NULL) {
    perror("map alloc 1");
    return 1;
  }
  for (size_t i = 0; i < BUFFER_SIZE; i++) {
    map[i] = (char *)malloc(sizeof(char) * BUFFER_SIZE);
  }
  size_t len_x = 0;
  size_t len_y = 0;

  char *line = malloc(BUFFER_SIZE);
  while (fgets(line, BUFFER_SIZE - 1, file)) {
    len_x = 0;

    int is_line_empty = 1;
    char *str = line;
    for (; *str && (char)*str != '\n'; str++) {
      char byte = *str;
      is_line_empty &= (byte == '.');
      map[len_y][len_x] = byte;
      len_x++;
    }
    len_y++;

    // duplicate lines if empty
    if (is_line_empty) {
      memcpy(map[len_y], map[len_y - 1], len_x);
      len_y++;
    }
  }

  // duplicate columns if empty
  for (int x = len_x - 1; x >= 0; x--) {
    int is_column_empty = 1;
    for (int y = 0; y < len_y; y++) {
      char byte = map[y][x];
      is_column_empty &= (byte == '.');
    }

    if (is_column_empty) {
      for (int y = 0; y < len_y; y++) {
        memcpy(map[y] + x + 1, map[y] + x, len_x - x);
      }
      len_x++;
    }
  }

  size_t(*galaxies)[2] = (size_t(*)[2])malloc(sizeof(size_t[2]) * BUFFER_SIZE);
  size_t len_glx = 0;

  // get galaxies position after expansion
  for (int y = 0; y < len_y; y++) {
    for (int x = 0; x < len_x; x++) {
      char byte = map[y][x];
      if (byte == '#') {
        size_t glx[2] = {x, y};
        memcpy(galaxies + len_glx, glx, sizeof(size_t) * 2);
        len_glx++;
      }
    }
  }

  size_t path_sum = galaxies_solve(galaxies, len_glx);

  printf("answer: %zu\n", path_sum);

  fclose(file);
  return 0;
}
