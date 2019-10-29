#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "the2.h"

int main() {
    Environment* environment;
    User *temp_user;


    environment = init_environment("test.inp");

    temp_user = get_user(*environment, 1);
    printf("User1: id: %d name: %s hash: %lu\n", temp_user->id, temp_user->name, hash_code(temp_user));

    print_connections(temp_user);

    temp_user = get_user(*environment, 2);
    printf("User2: id: %d name: %s hash: %lu\n", temp_user->id, temp_user->name, hash_code(temp_user));

    print_connections(temp_user);

     temp_user = get_user(*environment, 3);
    printf("User3: id: %d name: %s hash: %lu\n", temp_user->id, temp_user->name, hash_code(temp_user));

    print_connections(temp_user);

    temp_user = get_user(*environment, 4);
    printf("User4: id: %d name: %s hash: %lu\n", temp_user->id, temp_user->name, hash_code(temp_user));

    print_connections(temp_user);

    return 0;
}