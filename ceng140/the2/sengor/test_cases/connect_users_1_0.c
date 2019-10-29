#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "the2.h"

int main() {
    Environment* environment;
    User *user1, *user2, **iter;


    environment = init_environment("test1.inp");
    
    iter = environment->users;
    while(*iter!=NULL) {
        printf("User[id:%d, name: %s, hash: %lu]:\n", (*iter)->id, (*iter)->name, hash_code(*iter));
        iter+=1;
    }
    user1 = get_user(*environment, 1);

    user2 = get_user(*environment, 3);
    
    printf("Initial connections of User[id:%d, name: %s, hash: %lu]:\n", user1->id, user1->name, hash_code(user1));
    print_connections(user1);
    
    printf("Initial connections of User[id:%d, name: %s, hash: %lu]:\n", user2->id, user2->name, hash_code(user2));
    print_connections(user2);
    
    connect_users(*environment, 1, 3, 555);
    
    printf("Final connections of User[id:%d, name: %s, hash: %lu]:\n", user1->id, user1->name, hash_code(user1));
    print_connections(user1);
    printf("Final connections of User[id:%d, name: %s, hash: %lu]:\n", user2->id, user2->name, hash_code(user2));
    print_connections(user2);


    return 0;
}
