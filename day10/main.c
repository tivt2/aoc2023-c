#include <stdio.h>
#include <stdlib.h>

#define INPUT "day10/input"
#define TEST_INPUT "day10/test_input"

#define BUFFER_SIZE 1024

typedef enum {
  ANIMAL,
  TOP_DOWN,
  LEFT_RIGHT,
  TOP_RIGHT,
  TOP_LEFT,
  DOWN_RIGHT,
  DOWN_LEFT,
  NONE,
  INVALID,
} Symbol;

Symbol symbol_get(char byte) {
  switch (byte) {
  case 'S':
    return ANIMAL;
    break;
  case '|':
    return TOP_DOWN;
    break;
  case '-':
    return LEFT_RIGHT;
    break;
  case 'L':
    return TOP_RIGHT;
    break;
  case 'J':
    return TOP_LEFT;
    break;
  case 'F':
    return DOWN_RIGHT;
    break;
  case '7':
    return DOWN_LEFT;
    break;
  case '.':
    return NONE;
    break;
  default:
    return INVALID;
  }
}

typedef enum { TOP, RIGHT, DOWN, LEFT } Dir;

static size_t len_x = 0;
static size_t len_y = 0;
static size_t animal_idx = 0;

int idx(size_t x, size_t y, size_t *n) {
  if (x < 0 || x >= len_x || y < 0 || y >= len_y) {
    return -1;
  }
  *n = y * len_y + x;
  return 1;
}

int idx_get(size_t n, size_t *x, size_t *y) {
  if (n < 0 || n >= len_x * len_y) {
    return -1;
  }
  *x = n % len_y;
  *y = n / len_y;
  return 1;
}

int maze_is_valid_pipe(Symbol *maze, size_t pos, Dir dir, Dir *next_dir) {
  Symbol pipe = maze[pos];
  switch (dir) {
  case TOP:
    if (pipe == TOP_LEFT) {
      *next_dir = LEFT;
      return 1;
    } else if (pipe == TOP_RIGHT) {
      *next_dir = RIGHT;
      return 1;
    } else if (pipe == TOP_DOWN) {
      *next_dir = DOWN;
      return 1;
    }
    break;
  case RIGHT:
    if (pipe == LEFT_RIGHT) {
      *next_dir = LEFT;
      return 1;
    } else if (pipe == TOP_RIGHT) {
      *next_dir = TOP;
      return 1;
    } else if (pipe == DOWN_RIGHT) {
      *next_dir = DOWN;
      return 1;
    }
    break;
  case DOWN:
    if (pipe == DOWN_LEFT) {
      *next_dir = LEFT;
      return 1;
    } else if (pipe == TOP_DOWN) {
      *next_dir = TOP;
      return 1;
    } else if (pipe == DOWN_RIGHT) {
      *next_dir = RIGHT;
      return 1;
    }
    break;
  case LEFT:
    if (pipe == DOWN_LEFT) {
      *next_dir = DOWN;
      return 1;
    } else if (pipe == TOP_LEFT) {
      *next_dir = TOP;
      return 1;
    } else if (pipe == LEFT_RIGHT) {
      *next_dir = RIGHT;
      return 1;
    }
    break;
  }
  return 0;
}

size_t maze_walk(Symbol *maze, size_t pos, Dir dir) {
  Symbol curr = maze[pos];
  Dir next_dir;
  if (curr == ANIMAL) {
    return 1;
  } else if (!maze_is_valid_pipe(maze, pos, dir, &next_dir)) {
    return -1;
  }

  size_t x, y, next_pos;
  idx_get(pos, &x, &y);

  switch (next_dir) {
  case TOP:
    if (idx(x, y - 1, &next_pos) == 1) {
      return 1 + maze_walk(maze, next_pos, DOWN);
    }
    return -1;
    break;
  case RIGHT:
    if (idx(x + 1, y, &next_pos) == 1) {
      return 1 + maze_walk(maze, next_pos, LEFT);
    }
    return -1;
    break;
  case DOWN:
    if (idx(x, y + 1, &next_pos) == 1) {
      return 1 + maze_walk(maze, next_pos, TOP);
    }
    return -1;
    break;
  case LEFT:
    if (idx(x - 1, y, &next_pos) == 1) {
      return 1 + maze_walk(maze, next_pos, RIGHT);
    }
    return -1;
    break;
  default:
    return -1;
  }
}

size_t maze_solve(Symbol *maze, size_t animal) {
  size_t len = 0;

  size_t x, y, next_pos;
  idx_get(animal, &x, &y);
  if (idx(x, y - 1, &next_pos) == 1) {
    len = maze_walk(maze, next_pos, DOWN);
    if (len != -1) {
      return len;
    }
  }
  if (idx(x + 1, y, &next_pos) == 1) {
    len = maze_walk(maze, next_pos, LEFT);
    if (len != -1) {
      return len;
    }
  }
  if (idx(x, y + 1, &next_pos) == 1) {
    len = maze_walk(maze, next_pos, TOP);
    if (len != -1) {
      return len;
    }
  }
  if (idx(x - 1, y, &next_pos) == 1) {
    len = maze_walk(maze, next_pos, RIGHT);
    if (len != -1) {
      return len;
    }
  }
  return 0;
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  Symbol *maze = (Symbol *)malloc(sizeof(Symbol) * BUFFER_SIZE * BUFFER_SIZE);
  size_t animal_x = 0;
  size_t animal_y = 0;

  char *line = (char *)malloc(BUFFER_SIZE);
  while (fgets(line, BUFFER_SIZE - 1, file)) {
    size_t x = 0;
    char *entry = line;
    for (; *entry; entry++) {
      char byte = *entry;
      Symbol symbol = symbol_get(byte);
      if (symbol == INVALID) {
        continue;
      }

      if (symbol == ANIMAL) {
        animal_x = x;
        animal_y = len_y;
      }

      maze[len_y * len_x + x] = symbol;

      x++;
    }

    len_x = x;
    len_y++;
  }

  animal_idx = animal_y * len_x + animal_x;

  size_t len = maze_solve(maze, animal_idx) / 2;

  printf("answer: %zu\n", len);

  free(maze);
  free(line);
  fclose(file);
  return 0;
}
