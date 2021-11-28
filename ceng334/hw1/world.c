#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include "logging.h"
#include "message.h"

#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

typedef struct player_t {
	bool is_ready;
	bool is_alive;
	coordinate position;
	int pid;
	int fileno;
} player_t;

typedef struct monster_t {
	bool is_ready;
	bool is_alive;
	char symbol;
	coordinate position;
	int pid;
	int fileno;
} monster_t;

bool is_game_over = false;
int alive_monster_count;
int monster_number;
int room_width, room_height;
coordinate door_position;

int stat;

player_t player;
monster_t monsters[MONSTER_LIMIT];

void dfe(int *pid, int *fileno, char *args[]) {
	int fd[2];

	PIPE(fd);

	*fileno = fd[0];
	*pid = fork();

	if (*pid == -1) {
		exit(1);
	}
	else if (*pid == 0) {
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(fd[0]);

		dup2(fd[1], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);

		execvp(args[0], args);
	} else {
		close(fd[1]);
	}
}


void take_inputs() {
	char *s_room_width, *s_room_height, *s_door_x, *s_door_y, *s_player_executable_name, *s_player_x, *s_player_y,
			*s_player_max_monster_count, *s_player_max_distance, *s_player_turn_number;

	scanf(" %ms %ms", &s_room_width, &s_room_height);
	scanf(" %ms %ms", &s_door_x, &s_door_y);
	scanf(" %ms %ms", &s_player_x, &s_player_y);
	scanf(" %ms %ms %ms %ms", &s_player_executable_name, &s_player_max_monster_count,
		&s_player_max_distance, &s_player_turn_number);
	scanf(" %d", &monster_number);

	alive_monster_count = monster_number;

	room_width = atoi(s_room_width);
	room_height = atoi(s_room_height);
	door_position.x = atoi(s_door_x);
	door_position.y = atoi(s_door_y);

	player.is_alive = true;
	player.is_ready = false;
	player.position.x = atoi(s_player_x);
	player.position.y = atoi(s_player_y);

	char *p_args[] = {s_player_executable_name, s_door_x, s_door_y, s_player_max_monster_count,
							s_player_max_distance, s_player_turn_number, NULL};
	dfe(&player.pid, &player.fileno, p_args);

	free(s_room_width);
	free(s_room_height);
	free(s_door_x);
	free(s_door_y);

	free(s_player_executable_name);
	free(s_player_x);
	free(s_player_y);
	free(s_player_max_monster_count);
	free(s_player_max_distance);
	free(s_player_turn_number);


	char *s_monster_executable_name, c_monster_symbol, *s_monster_x, *s_monster_y, *s_monster_health, *s_monster_damage,
			*s_monster_defence, *s_monster_range;

	for (int i = 0; i < monster_number; i++) {
		scanf(" %ms %c %ms %ms %ms %ms %ms %ms", &s_monster_executable_name,
			&c_monster_symbol, &s_monster_x, &s_monster_y, &s_monster_health,
			&s_monster_damage, &s_monster_defence, &s_monster_range);

		monsters[i].is_alive = true;
		monsters[i].is_ready = false;
		monsters[i].symbol = c_monster_symbol;
		monsters[i].position.x = atoi(s_monster_x);
		monsters[i].position.y = atoi(s_monster_y);

		char *args[] = {s_monster_executable_name, s_monster_health, s_monster_damage,
							s_monster_defence, s_monster_range, NULL};
		dfe(&monsters[i].pid, &monsters[i].fileno, args);

		free(s_monster_executable_name);
		free(s_monster_x);
		free(s_monster_y);
		free(s_monster_health);
		free(s_monster_damage);
		free(s_monster_defence);
		free(s_monster_range);
	}
}


void check_ready_messages() {
	while (!player.is_ready) {
		player_response resp;

		read(player.fileno, &resp, sizeof(resp));

		if (resp.pr_type == pr_ready) {
			player.is_ready = true;
		}
	}

	for (int i = 0; i < monster_number; i++) {
		while (!monsters[i].is_ready) {
			monster_response resp;

			read(monsters[i].fileno, &resp, sizeof(resp));

			if (resp.mr_type == mr_ready) {
				monsters[i].is_ready = true;
			}
		}
	}
}

void sort_monsters() {
	bool ordered = false;

	while(!ordered) {
		ordered = true;

		for (int i = 1; i < monster_number; i++) {
			if (!monsters[i - 1].is_alive && monsters[i].is_alive ||
				monsters[i - 1].is_alive == monsters[i].is_alive &&
				(monsters[i - 1].position.x > monsters[i].position.x ||
					(monsters[i - 1].position.x == monsters[i].position.x &&
											monsters[i - 1].position.y > monsters[i].position.y))
			) {
				monster_t temp = monsters[i];
				monsters[i] = monsters[i - 1];
				monsters[i - 1] = temp;
				ordered = false;
			}
		}
	}
}


void generate_and_print_map() {
	map_info info = {
		.map_width = room_width,
		.map_height = room_height,
		.door = door_position,
		.player = player.position,
		.alive_monster_count = alive_monster_count
	};

	int index = 0;
	for (int i = 0; i < monster_number; i++) {
		if (monsters[i].is_alive) {
			info.monster_types[index] = monsters[i].symbol;
			info.monster_coordinates[index] = monsters[i].position;

			index++;
		}
	}

	print_map(&info);
}


void write_player(int total_damage) {
	player_message msg = {
		.new_position = player.position,
		.total_damage = total_damage,
		.alive_monster_count = alive_monster_count,
		.game_over = is_game_over
	};

	int index = 0;
	for (int i = 0; i < monster_number; i++) {
		if (monsters[i].is_alive) {
			msg.monster_coordinates[index++] = monsters[i].position;
		}
	}

	write(player.fileno, &msg, sizeof(msg));
}

void write_monster(int i, int damage) {
	monster_message msg = {
		.new_position = monsters[i].position,
		.damage = damage,
		.player_coordinate = player.position,
		.game_over = is_game_over
	};

	write(monsters[i].fileno, &msg, sizeof(msg));
}


void process_player(int *monster_damages) {
	player_response resp;

	int r = read(player.fileno, &resp, sizeof(resp));

	if (r == 0) {
		is_game_over = true;

		close(player.fileno);
	}
	else {
		if (resp.pr_type == pr_move) {
			coordinate wanted_coordinate = resp.pr_content.move_to;

			if (wanted_coordinate.x == door_position.x && wanted_coordinate.y == door_position.y) {
				player.position = wanted_coordinate;
				is_game_over = true;
			} else if (wanted_coordinate.x != 0 && wanted_coordinate.y != 0 ||
					wanted_coordinate.x != room_width -1 && wanted_coordinate.y != room_height - 1) {
				bool exists = false;

				for (int i = 0; i < monster_number; i++) {
					if (monsters[i].is_alive) {
						if (monsters[i].position.x == wanted_coordinate.x && monsters[i].position.y == wanted_coordinate.y) {
							exists = true;
							break;
						}
					}
				}

				if (!exists) {
					player.position = wanted_coordinate;
				}
			}
		}
		else if (resp.pr_type == pr_attack) {
			for (int i = 0; i < monster_number; i++) {
				if (monsters[i].is_alive) {
					monster_damages[i] = resp.pr_content.attacked[i];
				}
			}
		}
		else if (resp.pr_type == pr_dead) {
			player.is_alive = false;

			close(player.fileno);

			is_game_over = true;
		}
	}
}

void process_monster(int i, int *total_damage) {
	monster_response resp;
	read(monsters[i].fileno, &resp, sizeof(resp));

	if (resp.mr_type == mr_move) {
		coordinate wanted_coordinate = resp.mr_content.move_to;

		bool exists = false;

		if (player.position.x == wanted_coordinate.x && player.position.y == wanted_coordinate.y) {
			exists = true;
		}

		if (!exists) {
			for (int i = 0; i < monster_number; i++) {
				if (monsters[i].is_alive) {
					if (monsters[i].position.x == wanted_coordinate.x && monsters[i].position.y == wanted_coordinate.y) {
						exists = true;
						break;
					}
				}
			}
		}

		if (!exists) {
			monsters[i].position = wanted_coordinate;
		}
	}
	else if (resp.mr_type == mr_attack) {
		*total_damage += resp.mr_content.attack;
	}
	else if (resp.mr_type == mr_dead) {
		monsters[i].is_alive = false;

		close(monsters[i].fileno);

		waitpid(monsters[i].pid, &stat, 0);

		alive_monster_count--;

		if (alive_monster_count == 0) {
			is_game_over = true;
		}
	}
}


void print_finish() {
	if (!player.is_alive) {
		print_game_over(go_died);
	}
	else if (alive_monster_count == 0) {
		print_game_over(go_survived);
	}
	else if (player.position.x == door_position.x && player.position.y == door_position.y) {
		print_game_over(go_reached);
	} else {
		print_game_over(go_left);
	}
}


void start_loop() {
	int total_damage = 0;
	int monster_damages_from_player[monster_number];

	while (!is_game_over) {
		for (int i = 0; i < monster_number; i++) {
			monster_damages_from_player[i] = 0;
		}

		write_player(total_damage);
		process_player(monster_damages_from_player);

		if (is_game_over) break;

		total_damage = 0;

		for (int i = 0; i < monster_number; i++) {
			if (monsters[i].is_alive) {
				write_monster(i, monster_damages_from_player[i]);
			}
		}

		for (int i = 0; i < monster_number; i++) {
			if (monsters[i].is_alive) {
				process_monster(i, &total_damage);

				if (is_game_over) break;
			}
		}

		if (is_game_over) break;

		sort_monsters();

		generate_and_print_map();
	}

	sort_monsters();

	write_player(0);
	for (int i = 0; i < monster_number; i++) {
		if (monsters[i].is_alive) {
			write_monster(i, 0);
		}
	}

	close(player.fileno);
	waitpid(player.pid, &stat, 0);
	for (int i = 0; i < monster_number; i++) {
		if (monsters[i].is_alive) {
			close(monsters[i].fileno);
			waitpid(monsters[i].pid, &stat, 0);
		}
	}

	generate_and_print_map();

	print_finish();
}


int main() {
	take_inputs();

	check_ready_messages();

	sort_monsters();

	generate_and_print_map();

	start_loop();

	return 0;
}
