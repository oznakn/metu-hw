#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "the2.h"

int main() {
    Environment* environment;
    User *temp_user;
    

    environment = init_environment("test1.inp");

    temp_user = get_user(*environment, 1);
    printf("User1: id: %d name: %s hash: %lu\n", temp_user->id, temp_user->name, hash_code(temp_user));

    return 0;
}