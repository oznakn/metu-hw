#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "the2.h"

/*
 *  returns the order of the letter in the alphabet
 *  if given char is not an ascii letter, returns 0
 */
int letter_order(char c) {
    if (c < 91 && c > 64) {
        return c - 64;
    } else if (c < 123 && c > 96) {
        return c - 96;
    }
    return 0;
}
/*
 * Prints the connections of given user.
 */
void print_connections(User *user) {
    Connection *connection;
    int i;
    for (i = 0; i < BUCKET_SIZE; i++) {
        printf("Bucket %d: ", i);
        if (user->connections[i] != NULL) {
            connection = user->connections[i];
            do {
                printf("[user1: %s, user2: %s, connection_time: %ld] -> ", connection->user1->name,
                       connection->user2->name, connection->connection_time);

            } while ((connection = connection->next_connection) != NULL);
        }
        printf("NULL\n");


    }
}

/*
 * Returns the hash code of the user.
 */
unsigned long hash_code(User *user) {
    unsigned long hash = 0;
    int i = 0;

    while ((*user).name[i] != 0) {
        hash += letter_order((*user).name[i]) * pow(HASH_KEY, i);
        i++;
    }

    return hash;
}

/*
 * Returns the User with given id from environment.
 */
User *get_user(Environment environment, int id) {
    int i;
    User *user;

    for (i = 0; ; i++) {
        user = environment.users[i];

        if (user == NULL) break;
        else if (user->id == id) return user;
    }

    return NULL;
}


Connection *create_new_connection(User *user1, User *user2, long connection_time) {
    Connection *connection = malloc(sizeof(Connection));
    connection->user1 = user1;
    connection->user2 = user2;
    connection->connection_time = connection_time;
    connection->prev_connection = NULL;
    connection->next_connection = NULL;

    return connection;
}


Connection *insert_connection(Connection *connection, Connection *head_connection, Connection *prev_connection, Connection *next_connection) {
    if (next_connection == NULL) {
        if (prev_connection == NULL) {
            return connection;
        }
        else {
            prev_connection->next_connection = connection;
            connection->prev_connection = prev_connection;
        }
    }
    else {
        next_connection->prev_connection = connection;
        connection->next_connection = next_connection;

        if (prev_connection == NULL) {
            return connection;
        }
        else {
            prev_connection->next_connection = connection;
            connection->prev_connection = prev_connection;
        }
    }

    return head_connection;
}


void connect_users_helper(User *user, User *user_other, long connection_time) {
    unsigned long hash = hash_code(user_other) % BUCKET_SIZE;

    Connection *prev_connection = NULL;
    Connection *curr_connection = user->connections[hash];

    while (curr_connection != NULL) {
        if (curr_connection->connection_time >= connection_time) break;

        prev_connection = curr_connection;
        curr_connection = curr_connection->next_connection;
    }

    user->connections[hash] = insert_connection(create_new_connection(user, user_other, connection_time), user->connections[hash], prev_connection, curr_connection);
}


/*
 * Connects two user and registers the related connection objects to both users' connection hash tables.
 */
void connect_users(Environment environment, int id1, int id2, long connection_time) {
    User *user1 = get_user(environment, id1);
    User *user2 = get_user(environment, id2);
    
    Connection *connection = get_connection(environment, id1, id2);

    if (connection == NULL) {
        connect_users_helper(user1, user2, connection_time);
        connect_users_helper(user2, user1, connection_time);
    }
}

/*
 * Rstrip the given string.
 */

char *rstrip(char *string) {
  char *end;

  while(isspace((unsigned char)*string)){
    string++;
  }

  if(*string == 0) {
    return string;
  }

  end = string + strlen(string) - 1;
  while(end > string && isspace((unsigned char)*end)){
    end--;
  }

  end[1] = '\0';

  return string;
}


/*
 * Creates a new environment with the information in the given file that contains users
 * with connections and returns it.
 */
Environment *init_environment(char *user_file_name) {
    Environment *environment;
    User *user;
    FILE *fp;
    char *line = NULL;
    char *iter;
    char *sep = " ";
    size_t len = 0;
    size_t read;
    int id, id1, id2;
    long timestamp;
    char *name;

    int u_count = 0;

    environment = malloc(sizeof(Environment));
    environment->users = malloc(sizeof(User*));
    environment->users[0] = NULL;

    fp = fopen(user_file_name, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        line = rstrip(line);
        if (strcmp(line, "#Users#") == 0) {
            continue;
        } else if (strcmp(line, "#Connections#") == 0) {
            break;
        }
        user = malloc(sizeof(User));
        iter = strtok(line, sep);
        id = atoi(iter);
        iter = strtok(NULL, sep);
        name = malloc(sizeof(char)*strlen(iter));
        strcpy(name, iter);
        user->id=id;
        user->name=name;
        environment->users = realloc(environment->users, sizeof(User*)*(u_count+2));
        environment->users[u_count] = user;
        environment->users[u_count+1] = NULL;
        u_count++;
    }

    /* Uncomment after connect_users function implemented. */
    while ((read = getline(&line, &len, fp)) != -1) {
        line = rstrip(line);
        iter = strtok(line, sep);
        id1 = atoi(iter);
        iter = strtok(NULL, sep);
        id2 = atoi(iter);
        iter = strtok(NULL, sep);
        timestamp = atol(iter);
        connect_users(*environment, id1, id2, timestamp);

    }

    return environment;
}

/*
 * Returns the connection between users with id1 and id2. The connection
 * is the one stored in user1's hash table. If there is no such a connection returns
 * NULL.
 */
Connection *get_connection(Environment environment, int id1, int id2) {
    Connection *head_connection;
    unsigned long hash;

    User *user1 = get_user(environment, id1);
    User *user2 = get_user(environment, id2);

    hash = hash_code(user2) % BUCKET_SIZE;
    head_connection = user1->connections[hash];

    while (head_connection != NULL) {
        if ((head_connection->user1->id == id1 && head_connection->user2->id == id2) || (head_connection->user1->id == id2 && head_connection->user2->id == id1)) return head_connection;

        head_connection = head_connection->next_connection;
    }

    return NULL;
}


void remove_connection_helper(Environment environment, User *user, User *other_user) {
    unsigned long hash = hash_code(other_user) % BUCKET_SIZE;
    
    Connection *head_connection = user->connections[hash];
    Connection *connection = get_connection(environment, user->id, other_user->id);

    if (connection != NULL) {
        Connection *prev_connection = connection->prev_connection;
        Connection *next_connection = connection->next_connection;

        if (prev_connection != NULL) {
            prev_connection->next_connection = next_connection;
        }

        if (next_connection != NULL) {
            next_connection->prev_connection = prev_connection;
        }

        if (head_connection == connection) {
            user->connections[hash] = connection->next_connection;
        }

        free(connection);
    }
}

/*
 * Remove connection from the given user. Additionally, finds the other user and removes related connection
 * from her/his table also.
 */
void remove_connection(Environment environment, int id1, int id2) {
    User *user1 = get_user(environment, id1);
    User *user2 = get_user(environment, id2);

    remove_connection_helper(environment, user1, user2);
    remove_connection_helper(environment, user2, user1);
}

/*
 * Returns dynamic User array which contains common connections. The last element
 * of the array is NULL, which is mandatory for detecting the end.
 */
User **get_common_connections(Environment environment, User *user1, User *user2) {
    User *common_users[10000], **result;
    int i, common_users_length = 0;
    Connection *next, *found = NULL;

    for (i = 0; i < BUCKET_SIZE; i++) {
        next = user1->connections[i];

        while (next != NULL) {
            if (next->user1 == user1) {
                found = get_connection(environment, user2->id, next->user2->id);
            }
            else if (next->user2 == user1) {
                found = get_connection(environment, user2->id, next->user1->id);
            }
            else {
                found = NULL;
            }

            if (found != NULL) {
                if (found->user1 == user2) {
                    common_users[common_users_length++] = found->user2;
                }
                else if (found->user2 == user2) {
                    common_users[common_users_length++] = found->user1;
                }
            }

            next = next->next_connection;
        }
    }

    result = malloc((common_users_length + 1) * sizeof(User *));

    for (i = 0; i < common_users_length; i++) {
        result[i] = common_users[i];
    }

    result[i] = NULL;

    return result;
}


