#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "message.h"

int max(int a, int b) {
	return a > b ? a : b;
}

void write_monster(monster_response resp) {
	write(STDOUT_FILENO, &resp, sizeof(resp));
}

monster_message read_monster() {
	monster_message resp;
	read(STDIN_FILENO, &resp, sizeof(resp));
	return resp;
}

coordinate calc_new_coordinate(coordinate c, int action) {
	int x_p = 0;
	int y_p = 0;

	switch (action) {
		case 0: {
			y_p = -1;
			break;
		}

		case 1: {
			x_p = 1;
			y_p = -1;
			break;
		}

		case 2: {
			x_p = 1;
			break;
		}

		case 3: {
			x_p = 1;
			y_p = 1;
			break;
		}

		case 4: {
			y_p = 1;
			break;
		}

		case 5: {
			x_p = -1;
			y_p = 1;
			break;
		}

		case 6: {
			x_p = -1;
			break;
		}

		case 7: {
			x_p = -1;
			y_p = -1;
			break;
		}
	}


	coordinate r = {
		.x = c.x + x_p,
		.y = c.y + y_p
	};

	return r;
}

int calc_distance(coordinate c1, coordinate c2) {
	return abs(c1.x - c2.x) + abs(c1.y - c2.y);
}

int main(int argc, char *argv[]) {
	int health = atoi(argv[1]);
	int damage = atoi(argv[2]);
	int defence = atoi(argv[3]);
	int range = atoi(argv[4]);

	coordinate monster_coordinate, player_coordinate;
	bool is_game_over = false;

	monster_response resp = {
		.mr_type = mr_ready
	};
	write_monster(resp);

	while (!is_game_over) {
		monster_message msg = read_monster();

		monster_coordinate = msg.new_position;
		player_coordinate = msg.player_coordinate;
		is_game_over = msg.game_over;
		int player_damage = msg.damage;

		if (is_game_over) break;

		health -= max(0, player_damage - defence);

		if (health <= 0) {
			monster_response attack_resp = {
				.mr_type = mr_dead,
			};
			write_monster(attack_resp);

			is_game_over = true;
		}
		else if (calc_distance(player_coordinate, monster_coordinate) <= range) {
			monster_response_content content = {.attack = damage};
			monster_response attack_resp = {
				.mr_type = mr_attack,
				.mr_content = content
			};
			write_monster(attack_resp);
		}
		else {
			int best_choice = 0;
			int best_value = calc_distance(player_coordinate, calc_new_coordinate(monster_coordinate, 0));

			for (int i = 1; i < 8; i++) {
				int c = calc_distance(player_coordinate, calc_new_coordinate(monster_coordinate, i));

				if (best_value > c) {
					best_value = c;
					best_choice = i;
				}
			}

			coordinate new_monster_coordinate = calc_new_coordinate(monster_coordinate, best_choice);

			monster_response_content content = {.move_to = new_monster_coordinate};
			monster_response attack_resp = {
				.mr_type = mr_move,
				.mr_content = content
			};
			write_monster(attack_resp);
		}
	}

	return 0;
}
