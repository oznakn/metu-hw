#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include "the1_serve_cust.h"

#define MAX_CUST_REQUIREMENT 1000
#define MAX_SHIP_TYPE_COUNT 10
#define MAX_CUST_COUNT 10

/*
	+ Function arguments are:
		++ min_required_ships:		Array of minimum number of ships required for each customer.
		++ customer_count:			Customer count, which is also length of min_required_ships.
		++ available_captain_count:	The number of captains available in the harbour for shipment.

	+ Returns:
		The number of customers whose ship requirements are satisfied.
		Try to maximize the number of satisfied customers.
		sort_integer_array() function is given for you as a HINT. There should be a reason for that.

	+ "return -1" is just a placeholder. Please change it after you implemented this function.

    Examples:

    1)  min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 28   => returns 5
    2)  min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 27   => returns 5
    3)  min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 19	  => returns 4
    4)  min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 18	  => returns 4
    5)  min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 13	  => returns 3
    6)  min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 11	  => returns 3
    7)  min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 8	  => returns 2
	8)  min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 6    => returns 2
    9)  min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 5    => returns 1
    10) min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 2	  => returns 1
    11) min_required_ships = {5, 9, 2, 4, 7}       customer_count = 5      available_captain_count = 1	  => returns 0

*/
int serve_customers(int min_required_ships[], int customer_count, int available_captain_count) {
	int i;
	int max_served_customers_count = 0;
	int available_captain_left = available_captain_count;

	sort_integer_array(min_required_ships, customer_count);

	for (i = 0; i < customer_count; i++) {
		if (available_captain_left >= min_required_ships[i]) {
			max_served_customers_count++;

			available_captain_left -= min_required_ships[i];
			continue;
		}

		break;
	}

	return max_served_customers_count;
}


/*
	+ These are helper functions for you. You can sort an integer array with 'sort_integer_array()' function.
	+ This function implementation is given to you as a HINT.
	+ Please DO NOT change any lines of these functions.
*/
int comparator_function(const void * a, const void * b) {
	return ( *(int*)a - *(int*)b );
}

void sort_integer_array(int arr[], int size) {
	qsort(arr, size, sizeof(int), comparator_function);
}