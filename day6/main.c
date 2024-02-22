#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day6/input"
#define TEST_INPUT "day6/test_input"

#define BUFFER_SIZE 1024

int race_process(int race[2]) {
  printf("time: %d, dist: %d\n", race[0], race[1]);
  int sum = 0;
  int accel = 1;
  for (int i = 0; i < race[0]; i++) {
    int travel_time = race[0] - i;
    int speed = i * accel;
    if (speed * travel_time > race[1]) {
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

  int races[1024][2];
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

  while (time_line != NULL) {
    races[races_len++][0] = atoi(time_line);

    time_line = strtok_r(NULL, " ", &saveptr1);
  }

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

  size_t k = races_len;
  while (distance_line != NULL) {
    races[races_len - k][1] = atoi(distance_line);
    k--;

    distance_line = strtok_r(NULL, " ", &saveptr2);
  }

  int record_beat = 1;

  for (int i = 0; i < races_len; i++) {
    record_beat *= race_process(races[i]);
  }

  printf("answer: %d\n", record_beat);

  return 0;
}
