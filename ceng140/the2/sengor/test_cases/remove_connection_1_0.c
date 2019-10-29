#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "the2.h"

int main() {
    Environment* environment;
    User *user, **iter;


    environment = init_environment("test1.inp");
    
    iter = environment->users;
    while(*iter!=NULL) {
        user = *iter;
        printf("Initial connections of User[id:%d, name: %s, hash: %lu]:\n", user->id, user->name, hash_code(user));
        print_connections(user);

        iter+=1;
    }

    remove_connection(*environment, 1, 2);
    remove_connection(*environment, 4, 1);
    remove_connection(*environment, 3, 2);

    iter = environment->users;
    while(*iter!=NULL) {
        user = *iter;
        printf("Final connections of User[id:%d, name: %s, hash: %lu]:\n", user->id, user->name, hash_code(user));
            print_connections(user);

        iter+=1;
    }
    

    return 0;
}
