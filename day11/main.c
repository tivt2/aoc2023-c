#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day11/input"
#define TEST_INPUT "day11/test_input"

#define MAP_SIZE 256 * 256
#define GROWTH 1000000

uint64_t width;
uint64_t height;

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  uint64_t(*galaxies)[2] =
      (uint64_t(*)[2])malloc(sizeof(uint64_t[2]) * MAP_SIZE);
  uint64_t len_glx = 0;

  char *map = malloc(MAP_SIZE);
  uint64_t map_sz = fread(map, sizeof(char), MAP_SIZE, file);
  width = strstr(map, "\n") - map + 1;
  height = map_sz / width;

  int ept_rows[height];
  for (int i = 0; i < height; i++) {
    ept_rows[i] = 1;
  }
  int ept_cols[width - 1];
  for (int i = 0; i < width - 1; i++) {
    ept_cols[i] = 1;
  }

  char *str = map;
  for (; *str; str++) {
    char byte = *str;
    uint64_t curr_pos = str - map;
    uint64_t x = curr_pos % width;
    uint64_t y = curr_pos / width;

    if (byte == '#') {
      galaxies[len_glx][0] = x;
      galaxies[len_glx][1] = y;
      len_glx++;

      ept_cols[x] = 0;
      ept_rows[y] = 0;
    }
  }

  uint64_t distance = 0;

  for (int i = 0; i < len_glx - 1; i++) {
    uint64_t *start = galaxies[i];
    for (int j = i + 1; j < len_glx; j++) {
      uint64_t *target = galaxies[j];

      uint64_t min_x = min(start[0], target[0]);
      uint64_t max_x = max(start[0], target[0]);

      uint64_t min_y = min(start[1], target[1]);
      uint64_t max_y = max(start[1], target[1]);

      uint64_t n_cols = 0;
      for (int k = min_x; k < max_x; k++) {
        n_cols += (ept_cols[k] == 1);
      }

      uint64_t n_rows = 0;
      for (int k = min_y; k < max_y; k++) {
        n_cols += (ept_rows[k] == 1);
      }

      uint64_t dx = (max_x - min_x);
      uint64_t dy = (max_y - min_y);
      distance += dx + dy + (n_cols * (GROWTH - 1)) + (n_rows * (GROWTH - 1));
    }
  }

  printf("answer: %lu\n", distance);

  fclose(file);

  return 0;
}
