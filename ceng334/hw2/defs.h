#ifndef DEFS_H
#define DEFS_H

#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>

struct drone_t;


typedef struct llist_node_t {
	void *val;
	struct llist_node_t *prev;
	struct llist_node_t *next;
} llist_node_t;

typedef struct {
	llist_node_t *head;
	pthread_mutex_t lock;
} llist_t;

void llist_init(llist_t *);
bool llist_is_empty(llist_t *llist);
void llist_push(llist_t *, void *);
void *llist_pop(llist_t *);
void llist_remove(llist_t *, void *);
void llist_destroy(llist_t *);



typedef struct sthread_t {
	pthread_t thread;
	sem_t status_sem;
} sthread_t;

void sthread_init(sthread_t *);
void sthread_bind(sthread_t *, void *(*fn)(void *), void *);
void sthread_created(sthread_t *, pthread_cond_t *);
void sthread_wait(sthread_t *);
void sthread_finished(sthread_t *, pthread_cond_t *);
void sthread_block_exit(sthread_t *);
void sthread_destroy(sthread_t *);


typedef struct package_t {
	int sender_id;
	int sender_hub_id;
	int receiver_id;
	int receiver_hub_id;
	struct drone_t *drone; // note: not array, just pointer
} package_t;
void fill_package(package_t *, int, int, int, int);



typedef struct hub_def_t {
	int id;
	int incoming_size;
	int outgoing_size;
	int charging_size;
	int *distances;
	int *closest_hubs;
} hub_def_t;


typedef struct hub_t {
	hub_def_t *def;
	bool active;
	pthread_mutex_t active_mut;

	int incoming_used;
	int incoming_reserved;
	llist_t incoming_llist;

	int outgoing_used;
	llist_t outgoing_llist;

	int charging_used;
	int charging_reserved;
	llist_t charging_llist;

	pthread_mutex_t incoming_mut;
	pthread_cond_t incoming_has_space_cond;
	pthread_cond_t incoming_has_package_cond;

	pthread_mutex_t outgoing_mut;
	pthread_cond_t outgoing_has_space_cond;
	pthread_cond_t outgoing_has_package_cond;

	pthread_mutex_t charging_mut;
	pthread_cond_t charging_has_drone_cond;
	pthread_cond_t charging_has_space_cond;

	sthread_t sthread;
} hub_t;

void fill_hub(hub_t *, hub_def_t *);
void destroy_hub(hub_t *);



typedef struct sender_def_t {
	int id;
	int wait_amount;
	int hub_id;
	int total_package_count;
} sender_def_t;

typedef struct sender_t {
	sender_def_t *def;

	sthread_t sthread;
} sender_t;

void fill_sender(sender_t *, sender_def_t *);
void destroy_sender(sender_t *);


typedef struct receiver_def_t {
	int id;
	int wait_amount;
	int hub_id;
} receiver_def_t;

typedef struct receiver_t {
	receiver_def_t *def;
	sthread_t sthread;
} receiver_t;

void fill_receiver(receiver_t *, receiver_def_t *);
void destroy_receiver(receiver_t *);

typedef struct drone_def_t {
	int id;
	int speed;
	int initial_hub_id;
	int max_range;
} drone_def_t;

typedef struct drone_t {
	drone_def_t *def;

	int hub_id;
	pthread_mutex_t hub_id_mut;

	int range;
	long long last_range_calc_time;
	pthread_mutex_t range_mut;

	int hub_request;

	package_t *package; // note: not array, just pointer
	pthread_mutex_t package_hub_request_mut;

	pthread_cond_t package_hub_request_received_cond;

	sthread_t sthread;
} drone_t;

void fill_drone(drone_t *, drone_def_t *);
void destroy_drone(drone_t *);

#endif
