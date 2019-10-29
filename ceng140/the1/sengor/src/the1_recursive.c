#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "the1_recursive.h"

#define MAX_CUST_REQUIREMENT 1000
#define MAX_SHIP_TYPE_COUNT 10
#define MAX_CUST_COUNT 10

/*
	+ Function arguments are:
		++ ship_types:				Array of ship types. Ship type is decided by ship capacity, it is a conceptual thing.
		++ ship_type_count:			Length of ship types array.
		++ customer_requirement:	Load requirement for a single customer.

	+ Returns:
		The minimum number of ships that are required to satisfy given customer load requirement.

	+ Recursive code should work fine with smaller values.
	When values get larger, recursive function returns after some considerable time.

	+ "return -1" is just a placeholder. Please change it after you implemented this function.
*/

int smallest_way(int ship_types[], int ship_type_count, int max_index, int left_requirement, int min_ships_required) {
	int a, b;

	if (left_requirement == 0) {
		return min_ships_required;
	}

	if (left_requirement < 0 || max_index < 0) {
		return INT_MAX;
	}

	a = smallest_way(ship_types, ship_type_count, max_index - 1, left_requirement, min_ships_required);
	b = smallest_way(ship_types, ship_type_count, max_index, left_requirement - ship_types[max_index], min_ships_required + 1);

	return a < b ? a : b;
}

int min_ships_required_recursive(int ship_types[], int ship_type_count, int customer_requirement) {
	return smallest_way(ship_types, ship_type_count, ship_type_count - 1, customer_requirement, 0);
}