#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "the2.h"

int main() {
    Environment* environment;
    User *user1, *user2, **iter;
    Connection *connection;

    environment = init_environment("test1.inp");
    
    iter = environment->users;
    while(*iter!=NULL) {
        printf("User[id:%d, name: %s, hash: %lu]\n", (*iter)->id, (*iter)->name, hash_code(*iter));
        iter+=1;
    }
    connection = get_connection(*environment, 1, 4);
    printf("User1[id:%d, name: %s, hash: %lu], User2[id:%d, name: %s, hash: %lu], Connection Time: %ld\n", connection->user1->id, connection->user1->name, hash_code(connection->user1), connection->user2->id, connection->user2->name, hash_code(connection->user2), connection->connection_time);
    
    connection = get_connection(*environment, 2, 4);
    printf("Connection is NULL: %d\n", connection==NULL);

    return 0;
}
