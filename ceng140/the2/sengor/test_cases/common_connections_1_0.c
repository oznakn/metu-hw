#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "the2.h"

int main() {
    Environment* environment;
    User *user1, *user2, **iter;


    environment = init_environment("test1.inp");
    
    user1=get_user(*environment, 3);
    user2=get_user(*environment, 1);
    iter = get_common_connections(*environment, user1,user2);
    
    printf("Common connections of User[id:%d, name: %s, hash: %lu] and User[id:%d, name: %s, hash: %lu]:\n",  user1->id, user1->name, hash_code(user1),  user2->id, user2->name, hash_code(user2));
    while(*iter!=NULL) {
        printf("User[id:%d, name: %s, hash: %lu]:\n", (*iter)->id, (*iter)->name, hash_code((*iter)));

        iter+=1;
    }

   
    

    return 0;
}
