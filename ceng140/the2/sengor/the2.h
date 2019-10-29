#ifndef _THE_2_
#define _THE_2_

#define BUCKET_SIZE 10
#define HASH_KEY 7


struct connection;

typedef struct user {
    int id;
    char *name;
    struct connection *connections[BUCKET_SIZE];
} User;

typedef struct connection {
    struct connection *prev_connection;
    struct connection *next_connection;
    struct user *user1;
    struct user *user2;
    long connection_time;
} Connection;

typedef struct environment {
    struct user **users;
} Environment;


void print_connections(User* user);

/*
 * Returns the hash code of the user.
 */
unsigned long hash_code(User *user);

/*
 * Returns the User with given id from environment.
 */
User *get_user(Environment environment, int id);

/*
 * Connects two user and registers the related connection objects to both users' connection hash tables.
 */
void connect_users(Environment environment, int id1, int id2, long connection_time);

/*
 * Creates a new environment with the information in the given file that contains users
 * with connections and returns it. The last element of the dynamic users array is NULL,
 * which is mandatory for detecting the end.
 */
Environment *init_environment(char *user_file_name);

/*
 * Returns the connection between users with id1 and id2. The connection
 * is the one stored in user1's hash table. If there is no such a connection returns
 * NULL.
 */
Connection* get_connection(Environment environment, int id1, int id2);

/*
 * Remove connection from the given user. Additionally, finds the other user and removes related connection
 * from her/his table also.
 */
void remove_connection(Environment environment, int id1, int id2);

/*
 * Returns dynamic User array which contains common connections. The last element
 * of the array is NULL, which is mandatory for detecting the end.
 */
User **get_common_connections(Environment environment, User *user1, User *user2);


#endif