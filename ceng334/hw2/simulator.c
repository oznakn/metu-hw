#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "helper.h"
#include "writeOutput.h"

#include "defs.h"

#define DEBUG_MODE false

#define DEFAULT_TIMEOUT 500 // in ms

#if DEBUG_MODE
	#define DEBUG(args...) fprintf(stderr, ##args)
#else
	#define DEBUG(args...) {};
#endif

#if DEBUG_MODE
	#define ASSERT(val) assert(val)
#else
	#define ASSERT(val) {};
#endif

#define MLOCK(mut) pthread_mutex_lock(&(mut));
#define MUNLOCK(mut) pthread_mutex_unlock(&(mut));
#define CWAIT(cond, lock) pthread_cond_wait(&(cond), &(lock));
#define CSIGNAL(cond) pthread_cond_signal(&(cond));
#define CBROADCAST(cond) pthread_cond_broadcast(&(cond));
#define CTIMEDWAIT(cond, lock) \
	do { \
		struct timespec ts; \
		clock_gettime(CLOCK_REALTIME, &ts); \
		ts.tv_sec += (DEFAULT_TIMEOUT / 1000); \
		ts.tv_nsec += (DEFAULT_TIMEOUT % 1000) * 1000000; \
		if (ts.tv_nsec >= 1000000000) { \
			ts.tv_nsec -= 1000000000; \
			ts.tv_sec++; \
		} \
		pthread_cond_timedwait(&(cond), &(lock), &ts); \
	} while (0);


int hub_count;
int drone_count;

hub_t *hubs;
sender_t *senders;
receiver_t *receivers;
drone_t *drones;

int active_hub_count;
int active_sender_count;

pthread_mutex_t active_hub_count_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t active_sender_count_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t flow_mut = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t setup_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t finish_cond = PTHREAD_COND_INITIALIZER;


void *hub_fn(void *vargp) {
	hub_t *hub = (hub_t *) vargp;

	HubInfo hub_info;

	FillHubInfo(&hub_info, hub->def->id);
	WriteOutput(NULL, NULL, NULL, &hub_info, HUB_CREATED);

	sthread_created(&hub->sthread, &setup_cond);

	while (true) {
		package_t *package = NULL;
		drone_t *drone = NULL;

		// Update hub active status
		bool hub_active = false;

		MLOCK(hub->outgoing_mut);
			hub_active = hub_active || (hub->outgoing_used > 0);
		MUNLOCK(hub->outgoing_mut);

		MLOCK(hub->incoming_mut);
			hub_active = hub_active || (hub->incoming_reserved > 0);
			hub_active = hub_active || (hub->incoming_used > 0);
		MUNLOCK(hub->incoming_mut);

		MLOCK(active_sender_count_lock);
			hub_active = hub_active || (active_sender_count > 0);
		MUNLOCK(active_sender_count_lock);

		MLOCK(hub->active_mut);
			hub->active = hub_active;
		MUNLOCK(hub->active_mut);

		if (!hub_active) break;

		MLOCK(hub->outgoing_mut);
			// Wait until package deposited
			CTIMEDWAIT(hub->outgoing_has_package_cond, hub->outgoing_mut);

			// Select a package to send
			llist_node_t *head = hub->outgoing_llist.head;
			while (head != NULL) {
				package_t *head_package = (package_t *) head->val;

				if (head_package->drone == NULL) {
					package = head_package;
					break;
				}

				head = head->next;
			}
		MUNLOCK(hub->outgoing_mut);

		// If there is no package, skip
		if (package != NULL) {
			// Select a drone
			MLOCK(hub->charging_mut);
				if (hub->charging_used != 0) {
					llist_node_t *head = hub->charging_llist.head;
					ASSERT(head != NULL);

					while (head != NULL) {
						drone_t *head_drone = (drone_t *) head->val;

						MLOCK(head_drone->range_mut);
							MLOCK(head_drone->package_hub_request_mut);
								if (drone == NULL
									&& head_drone->hub_request == -1 && head_drone->package == NULL) {
									drone = head_drone;
								} else if (drone != NULL && drone->range < head_drone->range
									&& head_drone->hub_request == -1 && head_drone->package == NULL) {
									drone = head_drone;
								}
							MUNLOCK(head_drone->package_hub_request_mut);
						MUNLOCK(head_drone->range_mut);

						head = head->next;
					}
				}
			MUNLOCK(hub->charging_mut);

			if (drone != NULL) {
				package->drone = drone;

				MLOCK(drone->package_hub_request_mut);
					ASSERT(drone->hub_request == -1);
					ASSERT(drone->package == NULL);

					drone->package = package;

					CSIGNAL(drone->package_hub_request_received_cond);
				MUNLOCK(drone->package_hub_request_mut);
			} else {
				// Drone cannot found, request drone from somewhere else

				for (int i = 2; i <= hub_count; i++) { // 1 is the current hub
					hub_t *curr_hub = &hubs[hub->def->closest_hubs[i]];

					MLOCK(curr_hub->charging_mut);
						llist_node_t *head = curr_hub->charging_llist.head;
						while (head != NULL) {
							drone_t *head_drone = (drone_t *) head->val;

							MLOCK(head_drone->package_hub_request_mut);
								if (head_drone->hub_request == -1 && head_drone->package == NULL) {
									head_drone->hub_request = hub->def->id;
									drone = head_drone;

									DEBUG("hub %d			| requested drone %d from hub %d\n", hub->def->id, head_drone->def->id, curr_hub->def->id);

									CSIGNAL(head_drone->package_hub_request_received_cond);
								}
							MUNLOCK(head_drone->package_hub_request_mut);

							if (drone != NULL) {
								break;
							}

							head = head->next;
						}

					MUNLOCK(curr_hub->charging_mut);

					if (drone != NULL) {
						break;
					}
				}

				if (drone == NULL) wait(UNIT_TIME);
			}
		}
	}

	// Hub stopped
	MLOCK(active_hub_count_lock);
		--active_hub_count;
	MUNLOCK(active_hub_count_lock);

	FillHubInfo(&hub_info, hub->def->id);
	WriteOutput(NULL, NULL, NULL, &hub_info, HUB_STOPPED);
	DEBUG("hub %d			| stopped\n", hub->def->id);
	sthread_finished(&hub->sthread, &finish_cond);

	return NULL;
}

void *sender_fn(void *vargp) {
	sender_t *sender = (sender_t *) vargp;
	hub_t *sender_hub = &hubs[sender->def->hub_id];

	int sender_package_count = sender->def->total_package_count;

	SenderInfo sender_info;
	PackageInfo package_info;

	FillSenderInfo(&sender_info, sender->def->id, sender->def->hub_id, sender->def->total_package_count, NULL);
	WriteOutput(&sender_info, NULL, NULL, NULL, SENDER_CREATED);

	sthread_created(&sender->sthread, &setup_cond);

	while (sender_package_count != 0) {
		MLOCK(sender_hub->outgoing_mut);

			// Wait deposit
			while (sender_hub->outgoing_used >= sender_hub->def->outgoing_size) {
				CWAIT(sender_hub->outgoing_has_space_cond, sender_hub->outgoing_mut);
			}

			// Select a random hub to send
			// int receiver_id = (rand() % hub_count) + 1;
			int receiver_id = (sender->def->id + 1);
			if (receiver_id == hub_count + 1) receiver_id = 1;
			ASSERT(receiver_id != sender->def->id);
			hub_t *receiver_hub = &hubs[receivers[receiver_id].def->hub_id];

			// Send deposit
			package_t *package = malloc(sizeof(package_t));
			--sender_package_count;

			fill_package(package, sender->def->id, sender->def->hub_id, receiver_id, receiver_hub->def->id);
			++sender_hub->outgoing_used;
			llist_push(&sender_hub->outgoing_llist, (void *) package);

			DEBUG("sender %d		| sending package to %d\n", sender->def->hub_id, receiver_hub->def->id);

			// Write output
			FillPacketInfo(&package_info, sender->def->id, sender->def->hub_id, receiver_id, receiver_hub->def->id);
			FillSenderInfo(&sender_info, sender->def->id, sender->def->hub_id, sender_package_count , &package_info);
			WriteOutput(&sender_info, NULL, NULL, NULL, SENDER_DEPOSITED);

			// Inform hub
			CSIGNAL(sender_hub->outgoing_has_package_cond);

		MUNLOCK(sender_hub->outgoing_mut);

		wait(sender->def->wait_amount);
	}

	// Sender stopped
	MLOCK(active_sender_count_lock);
		--active_sender_count;
	MUNLOCK(active_sender_count_lock);

	// Write output
	FillSenderInfo(&sender_info, sender->def->id, sender->def->hub_id, 0, NULL);
	WriteOutput(&sender_info, NULL, NULL, NULL, SENDER_STOPPED);

	DEBUG("sender %d		| stopped\n", sender->def->hub_id);

	// Finish sthread
	sthread_finished(&sender->sthread, &finish_cond);

	return NULL;
}

void *receiver_fn(void *vargp) {
	receiver_t *receiver = (receiver_t *) vargp;
	hub_t *receiver_hub = &hubs[receiver->def->hub_id];

	bool receiver_hub_active = true;

	PackageInfo package_info;
	ReceiverInfo receiver_info;

	FillReceiverInfo(&receiver_info, receiver->def->id, receiver->def->hub_id, NULL);
	WriteOutput(NULL, &receiver_info, NULL, NULL, RECEIVER_CREATED);

	sthread_created(&receiver->sthread, &setup_cond);

	while (receiver_hub_active) {
		MLOCK(receiver_hub->incoming_mut);
			while (!llist_is_empty(&receiver_hub->incoming_llist)) {
				package_t *package = (package_t *) llist_pop(&receiver_hub->incoming_llist);
				ASSERT(package->receiver_hub_id == receiver_hub->def->id);

				FillPacketInfo(&package_info, package->sender_id, package->sender_hub_id, package->receiver_id, package->receiver_hub_id);
				FillReceiverInfo(&receiver_info, package->receiver_id, package->receiver_hub_id, &package_info);
				WriteOutput(NULL, &receiver_info, NULL, NULL, RECEIVER_PICKUP);

				DEBUG("receiver %d		| received package from %d\n", receiver->def->hub_id, package->sender_hub_id);

				--receiver_hub->incoming_used;
				--receiver_hub->incoming_reserved;
				ASSERT(receiver_hub->incoming_used >= 0);
				ASSERT(receiver_hub->incoming_reserved >= 0);

				CSIGNAL(receiver_hub->incoming_has_space_cond);

				free(package);
			}
			ASSERT(receiver_hub->incoming_used == 0);
			ASSERT(receiver_hub->incoming_reserved >= 0);
		MUNLOCK(receiver_hub->incoming_mut);

		wait(receiver->def->wait_amount);

		MLOCK(receiver_hub->active_mut);
			receiver_hub_active = receiver_hub->active;
		MUNLOCK(receiver_hub->active_mut);
	}

	FillReceiverInfo(&receiver_info, receiver->def->id, receiver->def->hub_id, NULL);
	WriteOutput(NULL, &receiver_info, NULL, NULL, RECEIVER_STOPPED);

	DEBUG("receiver %d		| stopped\n", receiver->def->hub_id);
	sthread_finished(&receiver->sthread, &finish_cond);

	return NULL;
}

void *drone_fn(void *vargp) {
	drone_t *drone = (drone_t *) vargp;

	int currTime;

	PackageInfo package_info;
	DroneInfo drone_info;

	FillDroneInfo(&drone_info, drone->def->id, drone->def->initial_hub_id, drone->def->max_range, NULL, 0);
	WriteOutput(NULL, NULL, &drone_info, NULL, DRONE_CREATED);

	sthread_created(&drone->sthread, &setup_cond);

	while (true) {
		hub_t *sender_hub = NULL;
		hub_t *target_hub = NULL;
		package_t *package = NULL;

		int distance_between_hubs;

		bool active_hubs_exist;

		MLOCK(active_hub_count_lock);
			active_hubs_exist = active_hub_count > 0;
		MUNLOCK(active_hub_count_lock);

		if (!active_hubs_exist) break;

		bool has_action_to_do = true;

		MLOCK(drone->package_hub_request_mut);
			if (drone->package == NULL && drone->hub_request == -1) {
				CTIMEDWAIT(drone->package_hub_request_received_cond, drone->package_hub_request_mut);
			}

			if (drone->package != NULL) {
				target_hub = &hubs[drone->package->receiver_hub_id];

				package = drone->package;
			} else if (drone->hub_request != -1) {
				target_hub = &hubs[drone->hub_request];

				MLOCK(drone->hub_id_mut);
					if (drone->hub_id == target_hub->def->id) {
						target_hub = NULL;
					}
				MUNLOCK(drone->hub_id_mut);
			}

			if (target_hub == NULL) {
				has_action_to_do = false;
			}
		MUNLOCK(drone->package_hub_request_mut);

		if (!has_action_to_do) continue;

		// Fetch some values
		MLOCK(drone->hub_id_mut);
			sender_hub = &hubs[drone->hub_id];
		MUNLOCK(drone->hub_id_mut);

		// calculate distance
		distance_between_hubs = target_hub->def->distances[sender_hub->def->id];
		ASSERT(distance_between_hubs > 0);

		// Update range
		MLOCK(drone->range_mut);
			currTime = timeInMilliseconds();
			drone->range = calculate_drone_charge(currTime - drone->last_range_calc_time, drone->range, drone->def->max_range);
			drone->last_range_calc_time = currTime;
		MUNLOCK(drone->range_mut);

		if (package != NULL) {
			// Package to deliver
			ASSERT(sender_hub->def->id != target_hub->def->id);
			ASSERT(package->sender_hub_id != package->receiver_hub_id);

			// Reserve charging place
			MLOCK(target_hub->charging_mut);
				while (target_hub->charging_reserved == target_hub->def->charging_size) {
					CWAIT(target_hub->charging_has_space_cond, target_hub->charging_mut);
				}

				++target_hub->charging_reserved;
			MUNLOCK(target_hub->charging_mut);

			// Reserve package place
			MLOCK(target_hub->incoming_mut);
				while (target_hub->incoming_reserved == target_hub->def->incoming_size) {
					CWAIT(target_hub->incoming_has_space_cond, target_hub->incoming_mut);
				}

				++target_hub->incoming_reserved;
			MUNLOCK(target_hub->incoming_mut);

			// Wait range
			MLOCK(drone->range_mut);
				// Wait for range
				if (distance_between_hubs > drone->range) {
					int missing_range = distance_between_hubs - drone->range;
					int needed_time = missing_range * UNIT_TIME;
					wait(needed_time);
				}

				// Update range
				currTime = timeInMilliseconds();
				drone->range = calculate_drone_charge(currTime - drone->last_range_calc_time, drone->range, drone->def->max_range);
				drone->last_range_calc_time = currTime;

				FillPacketInfo(&package_info, package->sender_id, package->sender_hub_id, package->receiver_id, package->receiver_hub_id);
				FillDroneInfo(&drone_info, drone->def->id, package->sender_hub_id, drone->range, &package_info, 0);
				WriteOutput(NULL, NULL, &drone_info, NULL, DRONE_PICKUP);
			MUNLOCK(drone->range_mut);

			// Remove package
			MLOCK(sender_hub->outgoing_mut);
				--sender_hub->outgoing_used;

				llist_remove(&sender_hub->outgoing_llist, package);

				CSIGNAL(sender_hub->outgoing_has_space_cond);
			MUNLOCK(sender_hub->outgoing_mut);


			// Remove from charging place
			MLOCK(sender_hub->charging_mut);
				--sender_hub->charging_used;
				--sender_hub->charging_reserved;

				ASSERT(sender_hub->charging_used >= 0);
				ASSERT(sender_hub->charging_reserved >= 0);

				llist_remove(&sender_hub->charging_llist, drone);
				DEBUG("drone %d in hub %d p	| remove drone from charging space of hub %d\n", drone->def->id, drone->hub_id, sender_hub->def->id);

				CSIGNAL(sender_hub->charging_has_space_cond);
			MUNLOCK(sender_hub->charging_mut);


			MLOCK(drone->hub_id_mut);
				// Travel
				DEBUG("drone %d in hub %d p	| travelling from %d to %d\n", drone->def->id, drone->hub_id, sender_hub->def->id, target_hub->def->id);
				travel(distance_between_hubs, drone->def->speed);
			MUNLOCK(drone->hub_id_mut);


			// Update range
			MLOCK(drone->range_mut);
				drone->range -= range_decrease(distance_between_hubs, drone->def->speed);
				ASSERT(drone->range > 0);
			MUNLOCK(drone->range_mut);

			// Update to new id
			MLOCK(drone->hub_id_mut);
				drone->hub_id = target_hub->def->id;
			MUNLOCK(drone->hub_id_mut);

			// Drone landing
			MLOCK(target_hub->charging_mut);
				++target_hub->charging_used;

				llist_push(&target_hub->charging_llist, drone);

				CSIGNAL(target_hub->charging_has_drone_cond);
			MUNLOCK(target_hub->charging_mut);

			// Put package
			MLOCK(target_hub->incoming_mut);
				++target_hub->incoming_used;

				llist_push(&target_hub->incoming_llist, package);

				CSIGNAL(target_hub->incoming_has_package_cond);

				DEBUG("drone %d in hub %d p	| package dropped from %d to %d\n", drone->def->id, drone->hub_id, package->sender_hub_id, package->receiver_hub_id);
			MUNLOCK(target_hub->incoming_mut);


			MLOCK(drone->range_mut);
				FillPacketInfo(&package_info, package->sender_id, package->sender_hub_id, package->receiver_id, package->receiver_hub_id);
				FillDroneInfo(&drone_info, drone->def->id, drone->hub_id, drone->range, &package_info, 0);
				WriteOutput(NULL, NULL, &drone_info, NULL, DRONE_DEPOSITED);
			MUNLOCK(drone->range_mut);

		} else {
			// request to come

			// Reserve charging place
			MLOCK(target_hub->charging_mut);
				while (target_hub->charging_reserved == target_hub->def->charging_size) {
					CWAIT(target_hub->charging_has_space_cond, target_hub->charging_mut);
				}

				++target_hub->charging_reserved;
			MUNLOCK(target_hub->charging_mut);

			// Range operations
			MLOCK(drone->range_mut);
				// Wait for range
				if (distance_between_hubs > drone->range) {
					int missing_range = distance_between_hubs - drone->range;
					int needed_time = missing_range * UNIT_TIME;
					wait(needed_time);
				}

				// Update range
				currTime = timeInMilliseconds();
				drone->range = calculate_drone_charge(currTime - drone->last_range_calc_time, drone->range, drone->def->max_range);
				drone->last_range_calc_time = currTime;

				FillDroneInfo(&drone_info, drone->def->id, drone->hub_id, drone->range, NULL, target_hub->def->id);
				WriteOutput(NULL, NULL, &drone_info, NULL, DRONE_GOING);
			MUNLOCK(drone->range_mut);

			// Remove from charging place
			MLOCK(sender_hub->charging_mut);
				--sender_hub->charging_used;
				--sender_hub->charging_reserved;

				ASSERT(sender_hub->charging_used >= 0);
				ASSERT(sender_hub->charging_reserved >= 0);

				llist_remove(&sender_hub->charging_llist, drone);
				DEBUG("drone %d in hub %d r	| remove drone from charging space of hub %d\n", drone->def->id, drone->hub_id, sender_hub->def->id);

				CSIGNAL(sender_hub->charging_has_space_cond);
			MUNLOCK(sender_hub->charging_mut);

			MLOCK(drone->hub_id_mut);
				// Travel
				DEBUG("drone %d in hub %d r	| travelling from %d to %d\n", drone->def->id, drone->hub_id, sender_hub->def->id, target_hub->def->id);
				travel(distance_between_hubs, drone->def->speed);
			MUNLOCK(drone->hub_id_mut);

			// Update range
			MLOCK(drone->range_mut);
				drone->range -= range_decrease(distance_between_hubs, drone->def->speed);
				ASSERT(drone->range > 0);
			MUNLOCK(drone->range_mut);

			// Update to new id
			MLOCK(drone->hub_id_mut);
				drone->hub_id = target_hub->def->id;
			MUNLOCK(drone->hub_id_mut);

			// Drone landing
			MLOCK(target_hub->charging_mut);
				++target_hub->charging_used;

				llist_push(&target_hub->charging_llist, drone);

				CSIGNAL(target_hub->charging_has_drone_cond);
			MUNLOCK(target_hub->charging_mut);

			// Write output
			MLOCK(drone->range_mut);
				FillDroneInfo(&drone_info, drone->def->id, drone->hub_id, drone->range, NULL, 0);
				WriteOutput(NULL, NULL, &drone_info, NULL, DRONE_ARRIVED);
			MUNLOCK(drone->range_mut);
		}

		MLOCK(drone->package_hub_request_mut);
			if (package != NULL) {
				drone->package = NULL;
			} else {
				drone->hub_request = -1;
			}
		MUNLOCK(drone->package_hub_request_mut);
	}

	FillDroneInfo(&drone_info, drone->def->id, drone->hub_id, drone->range, NULL, 0);
	WriteOutput(NULL, NULL, &drone_info, NULL,DRONE_STOPPED);
	DEBUG("drone %d			| stopped\n", drone->def->id);

	sthread_finished(&drone->sthread, &finish_cond);

	return NULL;
}


void take_input() {
	scanf(" %d", &hub_count);

	active_hub_count = hub_count;
	active_sender_count = hub_count;

	hubs = calloc(hub_count + 1, sizeof(hub_t));
	for (int i = 1; i <= hub_count; i++) {
		hub_def_t *hub_def = malloc(sizeof(hub_def_t));
		hub_def->id = i;

		scanf(" %d %d %d", &hub_def->incoming_size,
							&hub_def->outgoing_size,
							&hub_def->charging_size);

		hub_def->distances = calloc(hub_count + 1, sizeof(int));
		hub_def->closest_hubs = calloc(hub_count + 1, sizeof(int));
		for (int j = 1; j <= hub_count; j++) {
			scanf(" %d", &hub_def->distances[j]);
			hub_def->closest_hubs[j] = j;
		}

		// Bubble sort
		bool sorted = false;
		while (!sorted) {
			sorted = true;

			for (int j = 1; j < hub_count; j++) {
				int distance1 = hub_def->distances[hub_def->closest_hubs[j]];
				int distance2 = hub_def->distances[hub_def->closest_hubs[j + 1]];

				if (distance1 > distance2) {
					sorted = false;

					int temp = hub_def->closest_hubs[j];
					hub_def->closest_hubs[j] = hub_def->closest_hubs[j + 1];
					hub_def->closest_hubs[j + 1] = temp;
				}
			}
		}

		fill_hub(&hubs[i], hub_def);
	}

	senders = calloc(hub_count + 1, sizeof(sender_t));
	for (int i = 1; i <= hub_count; i++) {
		sender_def_t *sender_def = malloc(sizeof(sender_def_t));
		sender_def->id = i;

		scanf(" %d %d %d", &sender_def->wait_amount,
							&sender_def->hub_id,
							&sender_def->total_package_count);

		fill_sender(&senders[i], sender_def);
	}

	receivers = calloc(hub_count + 1, sizeof(receiver_t));
	for (int i = 1; i <= hub_count; i++) {
		receiver_def_t *receiver_def = malloc(sizeof(receiver_def_t));
		receiver_def->id = i;

		scanf(" %d %d", &receiver_def->wait_amount,
						&receiver_def->hub_id);

		fill_receiver(&receivers[i], receiver_def);
	}

	scanf(" %d", &drone_count);

	drones = calloc(drone_count + 1, sizeof(drone_t));
	for (int i = 1; i <= drone_count; i++) {
		drone_def_t *drone_def = malloc(sizeof(drone_def_t));
		drone_def->id = i;

		scanf(" %d %d %d", &drone_def->speed,
							&drone_def->initial_hub_id,
							&drone_def->max_range);

		fill_drone(&drones[i], drone_def);

		llist_push(&hubs[drone_def->initial_hub_id].charging_llist, &drones[i]);
		hubs[drone_def->initial_hub_id].charging_used++;
		hubs[drone_def->initial_hub_id].charging_reserved++;
	}
}

void initialize() {
	pthread_cond_init(&setup_cond, NULL);
	pthread_cond_init(&finish_cond, NULL);

	for (int i = 1; i <= hub_count; i++) {
		sthread_bind(&hubs[i].sthread, &hub_fn, (void *) &hubs[i]);
	}

	for (int i = 1; i <= hub_count; i++) {
		sthread_bind(&senders[i].sthread, &sender_fn, (void *) &senders[i]);
	}

	for (int i = 1; i <= hub_count; i++) {
		sthread_bind(&receivers[i].sthread, &receiver_fn, (void *) &receivers[i]);
	}

	for (int i = 1; i <= drone_count; i++) {
		sthread_bind(&drones[i].sthread, &drone_fn, (void *) &drones[i]);
	}

	pthread_cond_broadcast(&setup_cond);
}

void wait_threads() {
	for (int i = 1; i <= hub_count; i++) {
		sthread_wait(&hubs[i].sthread);
	}

	for (int i = 1; i <= hub_count; i++) {
		sthread_wait(&senders[i].sthread);
	}

	for (int i = 1; i <= hub_count; i++) {
		sthread_wait(&receivers[i].sthread);
	}

	for (int i = 1; i <= drone_count; i++) {
		sthread_wait(&drones[i].sthread);
	}

	pthread_cond_broadcast(&finish_cond);

	for (int i = 1; i <= hub_count; i++) {
		sthread_block_exit(&hubs[i].sthread);
	}

	for (int i = 1; i <= hub_count; i++) {
		sthread_block_exit(&senders[i].sthread);
	}

	for (int i = 1; i <= hub_count; i++) {
		sthread_block_exit(&receivers[i].sthread);
	}

	for (int i = 1; i <= drone_count; i++) {
		sthread_block_exit(&drones[i].sthread);
	}
}

void free_all() {
	for (int i = 1; i <= hub_count; i++) {
		destroy_hub(&hubs[i]);
	}

	for (int i = 1; i <= hub_count; i++) {
		destroy_sender(&senders[i]);
	}

	for (int i = 1; i <= hub_count; i++) {
		destroy_receiver(&receivers[i]);
	}

	for (int i = 1; i <= drone_count; i++) {
		destroy_drone(&drones[i]);
	}

	for (int i = 1; i <= hub_count; i++) {
		free(hubs[i].def->distances);
		free(hubs[i].def->closest_hubs);

		free(hubs[i].def);
		free(senders[i].def);
		free(receivers[i].def);
	}

	for (int i = 1; i <= drone_count; i++) {
		free(drones[i].def);
	}

	free(hubs);
	free(senders);
	free(receivers);
	free(drones);
}

int main() {
	srand(time(NULL));

	InitWriteOutput();

	take_input();

	initialize();

	wait_threads();

	free_all();

	return 0;
}
