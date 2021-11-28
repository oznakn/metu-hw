#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>

#include "helper.h"

#define DEBUG(args...) fprintf(stderr, ##args)


void llist_init(llist_t *llist) {
	llist->head = NULL;

	pthread_mutex_init(&llist->lock, NULL);
}

bool llist_is_empty(llist_t *llist) {
	pthread_mutex_lock(&llist->lock);
		bool r = llist->head == NULL;
	pthread_mutex_unlock(&llist->lock);

	return r;
}

void llist_push(llist_t *llist, void *val) {
	llist_node_t *node = malloc(sizeof(llist_node_t));
	node->val = val;

	pthread_mutex_lock(&llist->lock);
		if (llist->head == NULL) {
			node->prev = NULL;
			node->next = NULL;

			llist->head = node;
		} else {
			node->prev = NULL;
			node->next = llist->head;

			llist->head->prev = node;
			llist->head = node;
		}
	pthread_mutex_unlock(&llist->lock);
}

void *llist_pop(llist_t *llist) {
	pthread_mutex_lock(&llist->lock);
		llist_node_t *node = llist->head;

		if (node->next != NULL) {
			node->next->prev = NULL;
		}
		llist->head = node->next;
	pthread_mutex_unlock(&llist->lock);

	void *val = node->val;
	free(node);

	return val;
}

void llist_remove(llist_t *llist, void *val) {
	pthread_mutex_lock(&llist->lock);
		llist_node_t *node = llist->head;

		while (node != NULL) {
			if (node->val == val) {
				if (node == llist->head) {
					if (node->next) {
						node->next->prev = NULL;
					}

					llist->head = node->next;
				} else {
					if (node->next) {
						node->next->prev = node->prev;
					}

					if (node->prev) {
						node->prev->next = node->next;
					}
				}

				break;
			}

			node = node->next;
		}
	pthread_mutex_unlock(&llist->lock);

	free(node);
}

void llist_destroy(llist_t *llist) {
	while (!llist_is_empty(llist)) {
		llist_pop(llist);
	}

	pthread_mutex_destroy(&llist->lock);
}




void sthread_init(sthread_t *sthread) {
	sem_init(&sthread->status_sem, 0, 0);
}

void sthread_bind(sthread_t *sthread, void *(*fn)(void *), void *arg) {
	pthread_create(&sthread->thread, NULL, fn, arg);

	sem_wait(&sthread->status_sem);
}

void sthread_created(sthread_t *sthread, pthread_cond_t *wait_cond) {
	sem_post(&sthread->status_sem);

	pthread_cond_broadcast(wait_cond);
}

void sthread_wait(sthread_t *sthread) {
	sem_wait(&sthread->status_sem);
}

void sthread_finished(sthread_t *sthread, pthread_cond_t *wait_cond) {
	sem_post(&sthread->status_sem);

	pthread_cond_broadcast(wait_cond);
}

void sthread_block_exit(sthread_t *sthread) {
	pthread_join(sthread->thread, NULL);
}

void sthread_destroy(sthread_t *sthread) {
	sem_destroy(&sthread->status_sem);
}




void fill_package(package_t *package, int sender_id, int sender_hub_id, int receiver_id, int receiver_hub_id) {
	package->sender_id = sender_id;
	package->sender_hub_id = sender_hub_id;

	package->receiver_id = receiver_id;
	package->receiver_hub_id = receiver_hub_id;

	package->drone = NULL;
}


void fill_hub(hub_t *hub, hub_def_t *hub_def) {
	hub->def = hub_def;

	hub->active = true;
	pthread_mutex_init(&hub->active_mut, NULL);

	hub->incoming_used = 0;
	hub->incoming_reserved = 0;
	llist_init(&hub->incoming_llist);

	hub->outgoing_used = 0;
	llist_init(&hub->outgoing_llist);

	hub->charging_used = 0;
	hub->charging_reserved = 0;
	llist_init(&hub->charging_llist);

	pthread_mutex_init(&hub->incoming_mut, NULL);
	pthread_cond_init(&hub->incoming_has_space_cond, NULL);
	pthread_cond_init(&hub->incoming_has_package_cond, NULL);

	pthread_mutex_init(&hub->outgoing_mut, NULL);
	pthread_cond_init(&hub->outgoing_has_space_cond, NULL);
	pthread_cond_init(&hub->outgoing_has_package_cond, NULL);

	pthread_mutex_init(&hub->charging_mut, NULL);
	pthread_cond_init(&hub->charging_has_drone_cond, NULL);
	pthread_cond_init(&hub->charging_has_space_cond, NULL);

	sthread_init(&hub->sthread);
}

void destroy_hub(hub_t *hub) {
	sthread_destroy(&hub->sthread);

	pthread_mutex_lock(&hub->incoming_mut);
		while (!llist_is_empty(&hub->incoming_llist)) {
			package_t *package = (package_t *) llist_pop(&hub->incoming_llist);
			free(package);
		}
	pthread_mutex_unlock(&hub->incoming_mut);

	llist_destroy(&hub->incoming_llist);
	llist_destroy(&hub->outgoing_llist);
	llist_destroy(&hub->charging_llist);

	pthread_mutex_destroy(&hub->incoming_mut);
	pthread_cond_destroy(&hub->incoming_has_space_cond);
	pthread_cond_destroy(&hub->incoming_has_package_cond);

	pthread_mutex_destroy(&hub->outgoing_mut);
	pthread_cond_destroy(&hub->outgoing_has_space_cond);
	pthread_cond_destroy(&hub->outgoing_has_package_cond);

	pthread_mutex_destroy(&hub->charging_mut);
	pthread_cond_destroy(&hub->charging_has_drone_cond);
	pthread_cond_destroy(&hub->charging_has_space_cond);
}


void fill_sender(sender_t *sender, sender_def_t *sender_def) {
	sender->def = sender_def;

	sthread_init(&sender->sthread);
}

void destroy_sender(sender_t *sender) {
	sthread_destroy(&sender->sthread);
}


void fill_receiver(receiver_t *receiver, receiver_def_t *receiver_def) {
	receiver->def = receiver_def;

	sthread_init(&receiver->sthread);
}

void destroy_receiver(receiver_t *receiver) {
	sthread_destroy(&receiver->sthread);
}


void fill_drone(drone_t *drone, drone_def_t *drone_def) {
	drone->def = drone_def;

	drone->hub_id = drone->def->initial_hub_id;
	pthread_mutex_init(&drone->hub_id_mut, NULL);

	drone->range = drone->def->max_range;
	drone->last_range_calc_time = timeInMilliseconds();
	pthread_mutex_init(&drone->range_mut, NULL);

	drone->hub_request = -1;

	drone->package = NULL;
	pthread_mutex_init(&drone->package_hub_request_mut, NULL);

	pthread_cond_init(&drone->package_hub_request_received_cond, NULL);

	sthread_init(&drone->sthread);
}

void destroy_drone(drone_t *drone) {
	sthread_destroy(&drone->sthread);

	pthread_mutex_destroy(&drone->hub_id_mut);
	pthread_mutex_destroy(&drone->range_mut);
	pthread_mutex_destroy(&drone->package_hub_request_mut);

	pthread_cond_destroy(&drone->package_hub_request_received_cond);
}
