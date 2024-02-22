#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define INPUT "day5/input"
#define TEST_INPUT "day5/test_input"

#define BUFFER_SIZE 1024

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

typedef struct ListNode {
  uint64_t src;
  uint64_t dst;
  uint64_t len;
  int processed;
  struct ListNode *next;
} Node;

typedef struct {
  Node *head;
  Node *tail;
} List;

List *list_create(void) {
  List *list = (List *)malloc(sizeof(List));
  if (list == NULL) {
    return NULL;
  }

  list->head = NULL;
  list->tail = NULL;

  return list;
}

int list_push(List *list, uint64_t dst, uint64_t src, uint64_t len) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    return -1;
  }
  node->src = src;
  node->dst = dst;
  node->len = len;
  node->processed = 0;

  if (list->head == NULL) {
    list->head = node;
    list->tail = node;
    return 1;
  }

  list->tail->next = node;
  list->tail = node;
  return 1;
}

void list_print(List *list) {
  Node *curr = list->head;
  while (curr != NULL) {
    printf("dst: %lu, src: %lu, len: %lu\n", curr->dst, curr->src, curr->len);
    curr = curr->next;
  }
}

int file_list_create(FILE *file, char *buf, char *map, List *list) {
  char *saveptr1, *saveptr2;
  char *map_str = strtok_r(buf, " ", &saveptr1);
  if (strcmp(map, map_str) != 0) {
    return 0;
  }

  while ((fgets(buf, BUFFER_SIZE, file) != NULL) && strcmp(buf, "\n") != 0) {
    char *vals = strtok_r(buf, " ", &saveptr2);
    uint64_t dst = strtoul(vals, NULL, 10);
    vals = strtok_r(NULL, " ", &saveptr2);
    uint64_t src = strtoul(vals, NULL, 10);
    vals = strtok_r(NULL, " ", &saveptr2);
    uint64_t len = strtoul(vals, NULL, 10);

    list_push(list, dst, src, len);
  }

  return 1;
}

void seed_transport(List *seeds, Node *seed, List *map) {
  Node *transport = map->head;
  while (transport != NULL) {
    uint64_t seed_end = seed->dst + seed->len;
    uint64_t transport_end = transport->src + transport->len;

    if ((seed->dst < transport_end) && (transport->src < seed_end) &&
        !seed->processed) {
      int64_t delta_dst = transport->dst - transport->src;
      uint64_t start = max(seed->dst, transport->src);
      uint64_t end = min(seed_end, transport_end);

      if (seed->dst < (transport->src)) {
        list_push(seeds, seed->dst, 0, transport->src - seed->dst);
      }
      if (seed_end > transport_end) {
        list_push(seeds, transport_end, 0, seed_end - transport_end);
      }

      seed->dst = start + delta_dst;
      seed->len = end - start;
      seed->processed = 1;
    }

    transport = transport->next;
  }
}

void seeds_transport(List *seeds, List *map) {
  Node *seed = seeds->head;
  while (seed != NULL) {
    seed_transport(seeds, seed, map);

    seed = seed->next;
  }

  seed = seeds->head;
  while (seed != NULL) {
    seed->processed = 0;
    seed = seed->next;
  }
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

  List *seeds = list_create();

  char *saveptr;
  char *seeds_str = strtok_r(line, ":", &saveptr);
  while ((seeds_str = strtok_r(NULL, " \n", &saveptr)) != NULL) {
    int start = strtoul(seeds_str, NULL, 10);
    seeds_str = strtok_r(NULL, " \n", &saveptr);
    int len = strtoul(seeds_str, NULL, 10);
    list_push(seeds, start, 0, len);
  }

  if (fgets(line, BUFFER_SIZE, file) == NULL || (strcmp(line, "\n") != 0)) {
    perror("seeds new line");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("map start");
    return 1;
  }

  List *seed_to_soil = list_create();
  List *soil_to_fertilizer = list_create();
  List *fertilizer_to_water = list_create();
  List *water_to_light = list_create();
  List *light_to_temperature = list_create();
  List *temperature_to_humidity = list_create();
  List *humidity_to_location = list_create();

  if (!file_list_create(file, line, "seed-to-soil", seed_to_soil)) {
    perror("seed-to-soil");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("seed-to-soil new line");
    return 1;
  }

  if (!file_list_create(file, line, "soil-to-fertilizer", soil_to_fertilizer)) {
    perror("soil-to-fertilizer");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("soil-to-fertilizer new line");
    return 1;
  }

  if (!file_list_create(file, line, "fertilizer-to-water",
                        fertilizer_to_water)) {
    perror("fertilizer-to-water");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("fertilizer-to-water new line");
    return 1;
  }

  if (!file_list_create(file, line, "water-to-light", water_to_light)) {
    perror("water-to-light");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("water-to-light new line");
    return 1;
  }

  if (!file_list_create(file, line, "light-to-temperature",
                        light_to_temperature)) {
    perror("light-to-temperature");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("light-to-temperature new line");
    return 1;
  }

  if (!file_list_create(file, line, "temperature-to-humidity",
                        temperature_to_humidity)) {
    perror("temperature-to-humidity");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) == NULL) {
    perror("temperature-to-humidity new line");
    return 1;
  }

  if (!file_list_create(file, line, "humidity-to-location",
                        humidity_to_location)) {
    perror("humidity-to-location");
    return 1;
  }
  if (fgets(line, BUFFER_SIZE, file) != NULL) {
    perror("humidity-to-location new line");
    return 1;
  }

  seeds_transport(seeds, seed_to_soil);
  seeds_transport(seeds, soil_to_fertilizer);
  seeds_transport(seeds, fertilizer_to_water);
  seeds_transport(seeds, water_to_light);
  seeds_transport(seeds, light_to_temperature);
  seeds_transport(seeds, temperature_to_humidity);
  seeds_transport(seeds, humidity_to_location);

  uint64_t smallest_location = -1;

  Node *seed = seeds->head;
  while (seed != NULL) {
    if (smallest_location == -1) {
      smallest_location = seed->dst;
    } else {
      smallest_location = min(smallest_location, seed->dst);
    }
    seed = seed->next;
  }

  printf("answer: %lu\n", smallest_location);

  return 0;
}
