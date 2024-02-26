#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day11/input"
#define TEST_INPUT "day11/test_input"

#define BUFFER_SIZE 512
#define GROWTH 1000000

#define move_cost(c) (c == '*' ? GROWTH : 1)

uint64_t galaxies_solve(char **map, uint64_t (*glxs)[2], uint64_t len_glx) {
  uint64_t sum = 0;

  for (int i = 0; i < len_glx - 1; i++) {
    uint64_t start_x = glxs[i][0];
    uint64_t start_y = glxs[i][1];

    for (int j = i + 1; j < len_glx; j++) {
      uint64_t target_x = glxs[j][0];
      uint64_t target_y = glxs[j][1];

      while (start_x != target_x || start_y != target_y) {
        char look_u;
        char look_d;

        if (target_x > start_x) {
          char look_l = map[target_y][target_x - 1];
          if (target_y > start_y) {
            look_u = map[target_y - 1][target_x];
            if (look_l == '*') {
              sum += move_cost(look_u);
              target_y--;
            } else {
              sum += move_cost(look_l);
              target_x--;
            }
          } else if (target_y < start_y) {
            look_d = map[target_y + 1][target_x];
            if (look_l == '*') {
              sum += move_cost(look_d);
              target_y++;
            } else {
              sum += move_cost(look_l);
              target_x--;
            }
          } else {
            sum += move_cost(look_l);
            target_x--;
          }
        } else if (target_x < start_x) {
          char look_r = map[target_y][target_x + 1];
          if (target_y > start_y) {
            look_u = map[target_y - 1][target_x];
            if (look_r == '*') {
              sum += move_cost(look_u);
              target_y--;
            } else {
              sum += move_cost(look_r);
              target_x++;
            }
          } else if (target_y < start_y) {
            look_d = map[target_y + 1][target_x];
            if (look_r == '*') {
              sum += move_cost(look_d);
              target_y++;
            } else {
              sum += move_cost(look_r);
              target_x++;
            }
          } else {
            sum += move_cost(look_r);
            target_x++;
          }
        } else {
          if (target_y > start_y) {
            sum += move_cost(map[target_y - 1][target_x]);
            target_y--;
          } else if (target_y < start_y) {
            sum += move_cost(map[target_y + 1][target_x]);
            target_y++;
          }
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
  for (uint64_t i = 0; i < BUFFER_SIZE; i++) {
    map[i] = (char *)malloc(sizeof(char) * BUFFER_SIZE);
  }
  uint64_t len_x = 0;
  uint64_t len_y = 0;

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

    // mark line as 1000000 times bigger
    if (is_line_empty) {
      for (int x = 0; x < len_x; x++) {
        map[len_y - 1][x] = '*';
      }
    }
  }

  // mark column as 1000000 times bigger
  for (int x = len_x - 1; x >= 0; x--) {
    int is_column_empty = 1;
    for (int y = 0; y < len_y; y++) {
      char byte = map[y][x];
      is_column_empty &= (byte == '.' || byte == '*');
    }

    if (is_column_empty) {
      for (int y = 0; y < len_y; y++) {
        map[y][x] = '*';
      }
    }
  }

  uint64_t(*galaxies)[2] =
      (uint64_t(*)[2])malloc(sizeof(uint64_t[2]) * BUFFER_SIZE);
  uint64_t len_glx = 0;

  // get galaxies position after expansion
  for (int y = 0; y < len_y; y++) {
    for (int x = 0; x < len_x; x++) {
      char byte = map[y][x];
      if (byte == '#') {
        uint64_t glx[2] = {x, y};
        memcpy(galaxies + len_glx, glx, sizeof(uint64_t) * 2);
        len_glx++;
      }
    }
  }

  uint64_t path_sum = galaxies_solve(map, galaxies, len_glx);

  printf("answer: %zu\n", path_sum);

  fclose(file);
  return 0;
}
