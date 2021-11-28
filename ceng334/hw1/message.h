#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdbool.h>

#define MONSTER_LIMIT 20

typedef struct coordinate {
  int x, y;
} coordinate;

typedef struct player_message {
  coordinate new_position;
  int total_damage;
  int alive_monster_count;
  coordinate monster_coordinates[MONSTER_LIMIT];
  bool game_over;
} player_message;

typedef enum player_response_type {
  pr_ready,
  pr_move,
  pr_attack,
  pr_dead,
} player_response_type;

typedef union player_response_content {
  coordinate move_to;
  int attacked[MONSTER_LIMIT];
} player_response_content;

typedef struct player_response {
  player_response_type pr_type;
  player_response_content pr_content;
} player_response;

typedef struct monster_message {
  coordinate new_position;
  int damage;
  coordinate player_coordinate;
  bool game_over;
} monster_message;

typedef enum monster_response_type {
  mr_ready,
  mr_move,
  mr_attack,
  mr_dead,
} monster_response_type;

typedef union monster_response_content {
  coordinate move_to;
  int attack;
} monster_response_content;

typedef struct monster_response {
  monster_response_type mr_type;
  monster_response_content mr_content;
} monster_response;

#endif
