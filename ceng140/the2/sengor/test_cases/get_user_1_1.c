#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "the2.h"

int main() {
    Environment* environment;
    User *temp_user;
    

    environment = init_environment("test1.inp");

    temp_user = get_user(*environment, 5);
    printf("User is null: %d\n", temp_user==NULL);


    return 0;
}
