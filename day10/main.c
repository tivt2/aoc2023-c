#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day10/input"
#define TEST_INPUT "day10/test_input"

#define MAZE_SIZE 256 * 256

size_t maze_sz;
size_t w;

#define IDX(x, y) (y * w + x)

typedef enum { LEFT, RIGHT, UP, DOWN } Dir;

void maze_solve_rec(char *maze, size_t x, size_t y, Dir dir, size_t *idx_cache,
                    size_t *cache_len) {
  size_t pos = IDX(x, y);
  if (idx_cache[0] == pos) {
    return;
  }

  idx_cache[(*cache_len)++] = pos;

  switch (maze[pos]) {
  case '|':
    if (dir == UP) {
      maze_solve_rec(maze, x, y - 1, UP, idx_cache, cache_len);
    } else if (dir == DOWN) {
      maze_solve_rec(maze, x, y + 1, DOWN, idx_cache, cache_len);
    }
    break;
  case '-':
    if (dir == LEFT) {
      maze_solve_rec(maze, x - 1, y, LEFT, idx_cache, cache_len);
    } else if (dir == RIGHT) {
      maze_solve_rec(maze, x + 1, y, RIGHT, idx_cache, cache_len);
    }
    break;
  case 'L':
    if (dir == DOWN) {
      maze_solve_rec(maze, x + 1, y, RIGHT, idx_cache, cache_len);
    } else if (dir == LEFT) {
      maze_solve_rec(maze, x, y - 1, UP, idx_cache, cache_len);
    }
    break;
  case 'J':
    if (dir == DOWN) {
      maze_solve_rec(maze, x - 1, y, LEFT, idx_cache, cache_len);
    } else if (dir == RIGHT) {
      maze_solve_rec(maze, x, y - 1, UP, idx_cache, cache_len);
    }
    break;
  case '7':
    if (dir == UP) {
      maze_solve_rec(maze, x - 1, y, LEFT, idx_cache, cache_len);
    } else if (dir == RIGHT) {
      maze_solve_rec(maze, x, y + 1, DOWN, idx_cache, cache_len);
    }
    break;
  case 'F':
    if (dir == UP) {
      maze_solve_rec(maze, x + 1, y, RIGHT, idx_cache, cache_len);
    } else if (dir == LEFT) {
      maze_solve_rec(maze, x, y + 1, DOWN, idx_cache, cache_len);
    }
    break;
  default:
    printf("bad maze: %zu, %zu:%zu\n", pos, x, y);
    printf("%s\n", maze);
  }
}

void maze_loop_solve(char *maze, size_t x, size_t y, size_t *idx_cache,
                     size_t *cache_len) {
  idx_cache[(*cache_len)++] = IDX(x, y);

  size_t pos_l = IDX((x - 1), y);
  size_t pos_u = IDX(x, (y - 1));
  size_t pos_d = IDX(x, (y + 1));

  if (pos_l >= 0 &&
      (maze[pos_l] == '-' || maze[pos_l] == 'F' || maze[pos_l] == 'L')) {
    maze_solve_rec(maze, x - 1, y, LEFT, idx_cache, cache_len);
  } else if (pos_u >= 0 &&
             (maze[pos_u] == '|' || maze[pos_u] == '7' || maze[pos_u] == 'F')) {
    maze_solve_rec(maze, x, y - 1, UP, idx_cache, cache_len);
  } else if (pos_d < maze_sz &&
             (maze[pos_d] == '|' || maze[pos_d] == 'J' || maze[pos_d] == 'L')) {
    maze_solve_rec(maze, x, y + 1, DOWN, idx_cache, cache_len);
  } else {
    maze_solve_rec(maze, x + 1, y, RIGHT, idx_cache, cache_len);
  }
}

int maze_is_inside_loop(char *maze, size_t idx, size_t *pipe_idxs,
                        size_t pipe_len) {
  size_t x = idx % w;
  size_t y = idx / w;

  for (size_t i = 0; i < pipe_len; i++) {
    size_t pipe = pipe_idxs[i];
    if (idx == pipe) {
      return 0;
    }
  }

  size_t left_count = 0;
  int walk_x = x;

  while (walk_x >= 0) {
    size_t pos = IDX(walk_x, y);

    char cur_pipe = '*';
    for (size_t i = 0; i < pipe_len; i++) {

      if (pos == pipe_idxs[i]) {
        left_count++;
        cur_pipe = maze[pos];

        if (cur_pipe == '7') {
          while ((cur_pipe != 'F' && cur_pipe != 'L') && walk_x >= 0) {
            walk_x--;
            cur_pipe = maze[IDX(walk_x, y)];
          }
          if (cur_pipe == 'F') {
            left_count--;
          }
        } else if (cur_pipe == 'J') {
          while ((cur_pipe != 'F' && cur_pipe != 'L') && walk_x >= 0) {
            walk_x--;
            cur_pipe = maze[IDX(walk_x, y)];
          }
          if (cur_pipe == 'L') {
            left_count--;
          }
        }

        break;
      }
    }
    walk_x--;
  }

  return left_count % 2 == 1;
}

void maze_pipe_animal_infer(char *maze, size_t *pipe_idxs, size_t pipe_len) {
  size_t animal = pipe_idxs[0];
  size_t first = pipe_idxs[1];
  size_t last = pipe_idxs[pipe_len - 1];

  int first_walk = first - animal;
  int last_walk = animal - last;
  if (first_walk == w) {
    // down
    if (last_walk == 12) {
      maze[animal] = '|';
    } else if (last_walk == -1) {
      maze[animal] = 'F';
    } else {
      maze[animal] = '7';
    }
  } else if (first_walk == -w) {
    // up
    if (last_walk == -12) {
      maze[animal] = '|';
    } else if (last_walk == -1) {
      maze[animal] = 'L';
    } else {
      maze[animal] = 'J';
    }
  } else if (first_walk == 1) {
    // right
    if (last_walk == 1) {
      maze[animal] = '-';
    } else if (last_walk == 12) {
      maze[animal] = 'L';
    } else {
      maze[animal] = 'F';
    }
  } else if (first_walk == -1) {
    // left
    if (last_walk == -1) {
      maze[animal] = '-';
    } else if (last_walk == 12) {
      maze[animal] = 'J';
    } else {
      maze[animal] = '7';
    }
  }
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  char *maze = malloc(MAZE_SIZE);

  maze_sz = fread(maze, sizeof(char), MAZE_SIZE, file);
  size_t pipe_idxs[maze_sz];
  size_t pipe_len = 0;

  w = strstr(maze, "\n") - maze + 1;
  size_t animal = strstr(maze, "S") - maze;
  size_t animal_x = animal % w;
  size_t animal_y = animal / w;

  maze_loop_solve(maze, animal_x, animal_y, pipe_idxs, &pipe_len);

  maze_pipe_animal_infer(maze, pipe_idxs, pipe_len);

  size_t enclosed = 0;
  for (size_t i = 0; i < maze_sz; i++) {
    if (maze[i] == '\n') {
      continue;
    }

    enclosed += maze_is_inside_loop(maze, i, pipe_idxs, pipe_len);
  }

  printf("answer: %zu\n", enclosed);

  fclose(file);
  return 0;
}
