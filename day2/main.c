#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day2/input"
#define TEST_INPUT "day2/test_input"

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

typedef struct {
  int id;
  int red;
  int green;
  int blue;
} Game;

void game_round_points_parse(Game *game, char *round_str) {
  char *saveptr1, *saveptr2;
  char *points_str = strtok_r(round_str, ",", &saveptr1);

  while (points_str != NULL) {
    char *color_value = strtok_r(points_str, " ", &saveptr2);
    char *color = strtok_r(NULL, " ", &saveptr2);

    int value = atoi(color_value);
    if (strcmp(color, "red") == 0) {
      if (value > MAX_RED) {
        game->id = 0;
        return;
      }
      game->red += value;
    } else if (strcmp(color, "green") == 0) {
      if (value > MAX_GREEN) {
        game->id = 0;
        return;
      }
      game->green += value;
    } else if (strcmp(color, "blue") == 0) {
      if (value > MAX_BLUE) {
        game->id = 0;
        return;
      }
      game->blue += value;
    }

    points_str = strtok_r(NULL, ",", &saveptr1);
  }
}

void game_rounds_parse(Game *game, char *rounds_str) {
  char *round = strtok(rounds_str, ";");

  while (round != NULL) {
    game_round_points_parse(game, round);
    if (game->id == 0) {
      return;
    }
    round = strtok(NULL, ";\n");
  }
}

Game game_parse(char *game_str) {
  Game game;
  game.red = 0;
  game.green = 0;
  game.blue = 0;

  char *saveptr1, *saveptr2;
  char *indentifier = strtok_r(game_str, ":", &saveptr1);

  strtok_r(indentifier, " ", &saveptr2);
  char *id = strtok_r(NULL, "", &saveptr2);
  game.id = atoi(id);

  indentifier = strtok_r(NULL, "", &saveptr1);
  while (indentifier != NULL) {
    game_rounds_parse(&game, indentifier);
    if (game.id == 0) {
      break;
    }
    indentifier = strtok_r(NULL, "", &saveptr1);
  }
  return game;
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  int id_sum = 0;

  char line[1024];
  while ((fgets(line, sizeof(line), file)) != NULL) {
    Game game = game_parse(line);

    if (game.id != 0) {
      id_sum += game.id;
    }
    // printf("id: %d, red: %d, green: %d, blue: %d\n", game.id, game.red,
    //        game.green, game.blue);
    // printf("game id: %d\n", game.id);
    // printf("game red: %d\n", game.red);
    // printf("game green: %d\n", game.green);
    // printf("game blue: %d\n", game.blue);
  }

  printf("answer: %d\n", id_sum);

  return 0;
}
