#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day5/input"
#define TEST_INPUT "day5/test_input"

#define BUFFER_SIZE 1024

void map_range_process(size_t *seeds, size_t seeds_size, char *line,
                       int *processed) {
  char *vals = strtok(line, " ");
  size_t dst = strtoul(vals, NULL, 10);
  vals = strtok(NULL, " ");
  size_t src = strtoul(vals, NULL, 10);
  vals = strtok(NULL, " ");
  size_t len = strtoul(vals, NULL, 10);
  for (int i = 0; i < seeds_size; i++) {
    size_t seed = seeds[i];
    if ((src <= seed && seed < (src + len)) && !processed[i]) {
      size_t delta = seed - src;
      seeds[i] = dst + delta;
      processed[i] = 1;
    }
  }
}

int file_map_process(FILE *file, char *buf, char *map, size_t *seeds,
                     size_t seeds_size, int *processed) {
  char *map_str = strtok(buf, " ");
  if (strcmp(map, map_str) != 0) {
    return 0;
  }
  while ((fgets(buf, BUFFER_SIZE, file) != NULL) && strcmp(buf, "\n") != 0) {
    map_range_process(seeds, seeds_size, buf, processed);
  }
  for (int i = 0; i < seeds_size; i++) {
    processed[i] = 0;
  }
  return 1;
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  char line[BUFFER_SIZE];
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("seeds");
    return 1;
  }

  size_t seeds[BUFFER_SIZE];
  size_t seeds_size = 0;

  char *saveptr;
  char *seeds_str = strtok_r(line, ":", &saveptr);
  while ((seeds_str = strtok_r(NULL, " \n", &saveptr)) != NULL) {
    seeds[seeds_size++] = atoi(seeds_str);
  }

  // init boolean array
  int seeds_processed[seeds_size];
  for (int i = 0; i < seeds_size; i++) {
    seeds_processed[i] = 0;
  }

  if (fgets(line, BUFFER_SIZE, file) == NULL || (strcmp(line, "\n") != 0)) {
    perror("seeds empty line");
    return 1;
  }

  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("map start");
    return 1;
  }

  if (!file_map_process(file, line, "seed-to-soil", seeds, seeds_size,
                        seeds_processed)) {
    perror("seed-to-soil");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("seed-to-soil new line");
    return 1;
  }

  if (!file_map_process(file, line, "soil-to-fertilizer", seeds, seeds_size,
                        seeds_processed)) {
    perror("soil-to-fertilizer");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("soil-to-fertilizer new line");
    return 1;
  }

  if (!file_map_process(file, line, "fertilizer-to-water", seeds, seeds_size,
                        seeds_processed)) {
    perror("fertilizer-to-water");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("fertilizer-to-water new line");
    return 1;
  }

  if (!file_map_process(file, line, "water-to-light", seeds, seeds_size,
                        seeds_processed)) {
    perror("water-to-light");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("water-to-light new line");
    return 1;
  }

  if (!file_map_process(file, line, "light-to-temperature", seeds, seeds_size,
                        seeds_processed)) {
    perror("light-to-temperature");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("light-to-temperature new line");
    return 1;
  }

  if (!file_map_process(file, line, "temperature-to-humidity", seeds,
                        seeds_size, seeds_processed)) {
    perror("temperature-to-humidity");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("temperature-to-humidity new line");
    return 1;
  }

  if (!file_map_process(file, line, "humidity-to-location", seeds, seeds_size,
                        seeds_processed)) {
    perror("humidity-to-location");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) != NULL) {
    perror("humidity-to-location new line");
    return 1;
  }

  size_t smallest_location = -1;

  for (int i = 0; i < seeds_size; i++) {
    size_t location = seeds[i];
    if (smallest_location == -1) {
      smallest_location = location;
    } else {
      smallest_location =
          location < smallest_location ? location : smallest_location;
    }
  }

  printf("answer: %zu\n", smallest_location);

  return 0;
}
