#ifndef LOGGING_H
#define LOGGING_H

#include "message.h"

typedef struct map_info {
  int map_width, map_height;
  coordinate door;
  coordinate player;
  int alive_monster_count;
  char monster_types[MONSTER_LIMIT];
  coordinate monster_coordinates[MONSTER_LIMIT];
} map_info;

void print_map(map_info *mi);

typedef enum game_over_status {
  go_reached,
  go_survived,
  go_died,
  go_left,
} game_over_status;

void print_game_over(game_over_status go);

#endif
