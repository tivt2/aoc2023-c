#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day6/input"
#define TEST_INPUT "day6/test_input"

#define BUFFER_SIZE 1024

uint64_t race_process(uint64_t race[2]) {
  uint64_t sum = 0;
  for (uint64_t i = 0; i < race[0]; i++) {
    uint64_t travel_time = race[0] - i;
    if (i * travel_time > race[1]) {
      sum++;
    }
  }
  return sum;
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  uint64_t race[2];
  size_t races_len = 0;

  char line[BUFFER_SIZE];
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("time line read");
    return 1;
  }

  char *saveptr1, *saveptr2;
  char *time_line = strtok_r(line, ":", &saveptr1);
  if (strcmp(time_line, "Time") != 0) {
    perror("time line");
    return 1;
  }
  time_line = strtok_r(NULL, " ", &saveptr1);

  char time[1024];
  time[0] = '\0';
  while (time_line != NULL) {
    strcpy(time + strlen(time), time_line);

    time_line = strtok_r(NULL, " ", &saveptr1);
  }

  race[0] = strtoul(time, NULL, 10);

  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("distance line read");
    return 1;
  }

  char *distance_line = strtok_r(line, ":", &saveptr2);
  if (strcmp(distance_line, "Distance") != 0) {
    perror("distance line");
    return 1;
  }
  distance_line = strtok_r(NULL, " ", &saveptr2);

  char distance[1024];
  distance[0] = '\0';
  while (distance_line != NULL) {
    strcpy(distance + strlen(distance), distance_line);

    distance_line = strtok_r(NULL, " ", &saveptr2);
  }

  race[1] = strtoul(distance, NULL, 10);

  uint64_t record_beat = race_process(race);

  printf("answer: %lu\n", record_beat);

  return 0;
}
